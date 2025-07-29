#include "execute_cmd.h"
#include "builtin.h"
#include "path_utils.h"
#include "redirect_utils.h"
#include "process_utils.h"
#include "signal_handler.h"
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

/* Forward declarations */
static int	execute_simple_command(t_simple_command *simple);
static int	execute_pipeline(t_command *left, t_command *right);
static int	execute_binary_command(t_command *left, t_command *right, t_command_type type);

/* Main command execution dispatcher */
void	execute_command(t_command *command)
{
	int	exit_code;

	if (!command)
		return ;
	exit_code = 0;
	if (command->type == CM_SIMPLE && command->simple)
		exit_code = execute_simple_command(command->simple);
	else if (command->type == CM_PIPE)
		exit_code = execute_pipeline(command->left, command->right);
	else if (command->type == CM_AND || command->type == CM_OR)
		exit_code = execute_binary_command(command->left, command->right, command->type);
	exit_value(exit_code, SET);
}

/* Execute simple command */
static int	execute_simple_command(t_simple_command *simple)
{
	t_word_list			*words;
	char				*command_name;
	char				*command_path;
	t_redirect_backup	backup;
	int					exit_code;

	if (!simple || !simple->words || !simple->words->word || !simple->words->word->word)
		return (0);
	words = simple->words;
	command_name = words->word->word;
	init_redirect_backup(&backup);
	/* Setup redirections if any */
	if (simple->redirects)
	{
		if (setup_redirections(simple->redirects, &backup) == -1)
		{
			restore_redirections(&backup);
			return (1);
		}
	}
	
	/* Execute command */
	exit_code = 0;
	if (is_builtin_command(command_name))
		exit_code = execute_builtin(command_name, words->next);
	else
	{
		/* Find command in PATH */
		command_path = find_command_path(command_name);
		if (!command_path)
		{
			/* Check if command contains '/' to determine error message */
			if (ft_strchr(command_name, '/'))
			{
				/* For paths, check if file exists */
				if (access(command_name, F_OK) == -1)
				{
					ft_dprintf(STDERR_FILENO, ERROR_PREFIX "%s: No such file or directory\n", command_name);
					exit_code = 127;
				}
				else if (access(command_name, X_OK) == -1)
				{
					ft_dprintf(STDERR_FILENO, ERROR_PREFIX "%s: Permission denied\n", command_name);
					exit_code = 126;
				}
				else
				{
					ft_dprintf(STDERR_FILENO, ERROR_PREFIX "%s: command not found\n", command_name);
					exit_code = 127;
				}
			}
			else
			{
				ft_dprintf(STDERR_FILENO, ERROR_PREFIX "%s: command not found\n", command_name);
				exit_code = 127;
			}
		}
		else
		{
			/* Execute external command with fork/exec */
			exit_code = execute_external_command(command_path, command_name, words->next);
			free(command_path);
		}
	}
	
	/* Restore original file descriptors */
	if (simple->redirects)
		restore_redirections(&backup);
	return (exit_code);
}

/* Execute pipeline */
static int	execute_pipeline(t_command *left, t_command *right)
{
	int		pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		left_status;
	int		right_status;

	if (!left || !right)
		return (1);

	/* Create pipe */
	if (pipe(pipe_fd) == -1)
	{
		ft_dprintf(STDERR_FILENO, ERROR_PREFIX "pipe failed: %s\n", strerror(errno));
		return (1);
	}

	/* Execute left command */
	left_pid = fork();
	if (left_pid == -1)
	{
		ft_dprintf(STDERR_FILENO, ERROR_PREFIX "fork failed: %s\n", strerror(errno));
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (1);
	}
	else if (left_pid == 0)
	{
		/* Child process for left command - reset signals to default */
		setup_child_signals();
		
		close(pipe_fd[0]);					/* Close read end */
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			ft_dprintf(STDERR_FILENO, ERROR_PREFIX "dup2 failed: %s\n", strerror(errno));
			close(pipe_fd[1]);
			exit(1);
		}
		close(pipe_fd[1]);					/* Close write end after dup2 */
		execute_command(left);				/* Execute left command */
		exit(exit_value(0, GET));
	}

	/* Execute right command */
	right_pid = fork();
	if (right_pid == -1)
	{
		ft_dprintf(STDERR_FILENO, ERROR_PREFIX "fork failed: %s\n", strerror(errno));
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		kill(left_pid, SIGTERM);			/* Terminate left child */
		waitpid(left_pid, NULL, 0);
		return (1);
	}
	else if (right_pid == 0)
	{
		/* Child process for right command - reset signals to default */
		setup_child_signals();
		
		close(pipe_fd[1]);					/* Close write end */
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		{
			ft_dprintf(STDERR_FILENO, ERROR_PREFIX "dup2 failed: %s\n", strerror(errno));
			close(pipe_fd[0]);
			exit(1);
		}
		close(pipe_fd[0]);					/* Close read end after dup2 */
		execute_command(right);				/* Execute right command */
		exit(exit_value(0, GET));
	}

	/* Parent process: close pipe and wait for children */
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	
	/* Ignore signals while children are running */
	setup_parent_signals();
	
	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, &right_status, 0);

	/* Restore original signal handlers after children complete */
	set_signal();

	/* Return exit status of right command (bash behavior) */
	if (WIFEXITED(right_status))
		return (WEXITSTATUS(right_status));
	else if (WIFSIGNALED(right_status))
		return (128 + WTERMSIG(right_status));
	
	return (1);
}

/* 
 * Execute binary command (AND/OR operators)
 * 
 * Implements short-circuit evaluation:
 * - AND (&&): Execute right only if left succeeds (exit code 0)
 * - OR (||):  Execute right only if left fails (exit code != 0)
 * 
 * Returns: Final exit code of the last executed command
 */
static int	execute_binary_command(t_command *left, t_command *right, t_command_type type)
{
	int	left_exit;
	int	final_exit;
	
	/* Validate input parameters */
	if (!left)
	{
		ft_dprintf(STDERR_FILENO, ERROR_PREFIX "binary command: missing left operand\n");
		return (1);
	}
	
	if (type != CM_AND && type != CM_OR)
	{
		ft_dprintf(STDERR_FILENO, ERROR_PREFIX "binary command: invalid operator type\n");
		return (1);
	}
	
	/* Execute left command first */
	execute_command(left);
	left_exit = exit_value(0, GET);
	final_exit = left_exit;
	
	if (type == CM_AND)
	{
		/* AND operator: Execute right only if left succeeded (exit code 0) */
		if (left_exit == 0)
		{
			if (right)
			{
				execute_command(right);
				final_exit = exit_value(0, GET);
			}
			/* If no right command, return left's success status */
		}
		/* If left failed, return left's failure status without executing right */
	}
	else if (type == CM_OR)
	{
		/* OR operator: Execute right only if left failed (exit code != 0) */
		if (left_exit != 0)
		{
			if (right)
			{
				execute_command(right);
				final_exit = exit_value(0, GET);
			}
			/* If no right command, return left's failure status */
		}
		/* If left succeeded, return left's success status without executing right */
	}
	return (final_exit);
}
