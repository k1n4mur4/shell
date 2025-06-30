#include "execute_cmd.h"
#include "builtin.h"
#include "path_utils.h"
#include "redirect_utils.h"
#include "process_utils.h"

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
	
	/* Initialize redirection backup */
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
			ft_dprintf(STDERR_FILENO, "minishell: %s: command not found\n", command_name);
			exit_code = 127;
		}
		else
		{
			/* Execute external command with fork/exec */
			exit_code = execute_external_command(command_path, words->next);
			free(command_path);
		}
	}
	
	/* Restore original file descriptors */
	if (simple->redirects)
		restore_redirections(&backup);
	
	return (exit_code);
}

/* Execute pipeline (placeholder) */
static int	execute_pipeline(t_command *left, t_command *right)
{
	(void)left;
	(void)right;
	ft_dprintf(STDERR_FILENO, "Pipeline execution not yet implemented\n");
	return (1);
}

/* Execute binary command (AND/OR) */
static int	execute_binary_command(t_command *left, t_command *right, t_command_type type)
{
	int	left_exit;
	
	if (!left)
		return (1);
	
	/* Execute left command first */
	execute_command(left);
	left_exit = exit_value(0, GET);
	
	if (type == CM_AND)
	{
		/* Execute right only if left succeeded */
		if (left_exit == 0 && right)
			execute_command(right);
		return (exit_value(0, GET));
	}
	else if (type == CM_OR)
	{
		/* Execute right only if left failed */
		if (left_exit != 0 && right)
			execute_command(right);
		return (exit_value(0, GET));
	}
	
	return (left_exit);
}
