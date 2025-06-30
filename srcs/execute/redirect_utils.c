#include "redirect_utils.h"

/* Initialize redirection backup structure */
void	init_redirect_backup(t_redirect_backup *backup)
{
	backup->stdin_backup = -1;
	backup->stdout_backup = -1;
	backup->stderr_backup = -1;
}

/* Print redirection error message */
void	print_redirect_error(const char *filename, const char *error_msg)
{
	ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", filename, error_msg);
}

/* Handle input redirection (<) */
int	handle_input_redirect(const char *filename)
{
	int	fd;

	if (!filename)
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		print_redirect_error(filename, strerror(errno));
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		print_redirect_error(filename, "dup2 failed");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/* Handle output redirection (>) */
int	handle_output_redirect(const char *filename)
{
	int	fd;

	if (!filename)
		return (-1);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		print_redirect_error(filename, strerror(errno));
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		print_redirect_error(filename, "dup2 failed");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/* Handle append redirection (>>) */
int	handle_append_redirect(const char *filename)
{
	int	fd;

	if (!filename)
		return (-1);
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		print_redirect_error(filename, strerror(errno));
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		print_redirect_error(filename, "dup2 failed");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/* Handle heredoc redirection (<<) */
int	handle_heredoc_redirect(const char *delimiter)
{
	int		pipe_fd[2];
	char	*line;
	size_t	delim_len;

	if (!delimiter)
		return (-1);
	if (pipe(pipe_fd) == -1)
	{
		print_redirect_error("heredoc", "pipe creation failed");
		return (-1);
	}
	delim_len = ft_strlen(delimiter);
	ft_dprintf(STDOUT_FILENO, "> ");
	while ((line = readline("")) != NULL)
	{
		if (ft_strncmp(line, delimiter, delim_len) == 0 && 
			ft_strlen(line) == delim_len)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
		ft_dprintf(STDOUT_FILENO, "> ");
	}
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		print_redirect_error("heredoc", "dup2 failed");
		close(pipe_fd[0]);
		return (-1);
	}
	close(pipe_fd[0]);
	return (0);
}

/* Setup all redirections for a command */
int	setup_redirections(t_redirect *redirects, t_redirect_backup *backup)
{
	t_redirect	*current;

	if (!backup)
		return (-1);
	
	/* Backup current file descriptors */
	backup->stdin_backup = dup(STDIN_FILENO);
	backup->stdout_backup = dup(STDOUT_FILENO);
	backup->stderr_backup = dup(STDERR_FILENO);
	
	/* Apply each redirection */
	current = redirects;
	while (current)
	{
		if (current->type == R_INPUT)
		{
			if (handle_input_redirect(current->filename) == -1)
				return (-1);
		}
		else if (current->type == R_OUTPUT)
		{
			if (handle_output_redirect(current->filename) == -1)
				return (-1);
		}
		else if (current->type == R_APPEND)
		{
			if (handle_append_redirect(current->filename) == -1)
				return (-1);
		}
		else if (current->type == R_HEREDOC)
		{
			if (handle_heredoc_redirect(current->filename) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}

/* Restore original file descriptors */
void	restore_redirections(t_redirect_backup *backup)
{
	if (!backup)
		return ;
	
	if (backup->stdin_backup != -1)
	{
		dup2(backup->stdin_backup, STDIN_FILENO);
		close(backup->stdin_backup);
	}
	if (backup->stdout_backup != -1)
	{
		dup2(backup->stdout_backup, STDOUT_FILENO);
		close(backup->stdout_backup);
	}
	if (backup->stderr_backup != -1)
	{
		dup2(backup->stderr_backup, STDERR_FILENO);
		close(backup->stderr_backup);
	}
}