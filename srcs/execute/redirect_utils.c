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

/* Create temporary file for heredoc */
char	*create_temp_file(void)
{
	char	*temp_filename;
	char	*pid_str;
	char	*counter_str;
	char	*temp_part;
	int		pid;
	int		fd;
	static int	counter = 0;

	pid = getpid();
	pid_str = ft_itoa(pid);
	if (!pid_str)
		return (NULL);
	
	counter_str = ft_itoa(counter++);
	if (!counter_str)
	{
		free(pid_str);
		return (NULL);
	}
	
	temp_part = ft_strjoin(pid_str, "_");
	free(pid_str);
	if (!temp_part)
	{
		free(counter_str);
		return (NULL);
	}
	
	pid_str = ft_strjoin(temp_part, counter_str);
	free(temp_part);
	free(counter_str);
	if (!pid_str)
		return (NULL);
	
	temp_filename = ft_strjoin("/tmp/.minishell_heredoc_", pid_str);
	free(pid_str);
	if (!temp_filename)
		return (NULL);
	
	/* Create the temporary file */
	fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(temp_filename);
		return (NULL);
	}
	close(fd);
	
	/* Add to temporary file list for cleanup */
	add_temp_file(temp_filename);
	
	return (temp_filename);
}

/* Write heredoc content to temporary file */
int	write_heredoc_to_temp(const char *delimiter, const char *temp_filename)
{
	int		fd;
	char	*line;
	size_t	delim_len;
	int		success;

	if (!delimiter || !temp_filename)
		return (-1);
	
	fd = open(temp_filename, O_WRONLY | O_APPEND);
	if (fd == -1)
	{
		print_redirect_error(temp_filename, strerror(errno));
		return (-1);
	}

	delim_len = ft_strlen(delimiter);
	success = 0;
	while ((line = readline("> ")) != NULL)
	{
		if (ft_strncmp(line, delimiter, delim_len) == 0 && 
			ft_strlen(line) == delim_len)
		{
			free(line);
			success = 1;
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	
	/* If readline returned NULL (Ctrl+D or EOF), it's still valid */
	/* We only consider it an error if there was an actual failure */
	return (0);
}

/* Static temporary file list */
static t_temp_file	*g_temp_files = NULL;

/* Add temporary file to the list */
void	add_temp_file(const char *filename)
{
	t_temp_file	*new_file;
	
	if (!filename)
		return ;
	
	new_file = ft_calloc(1, sizeof(t_temp_file));
	if (!new_file)
		return ;
	
	new_file->filename = ft_strdup(filename);
	if (!new_file->filename)
	{
		free(new_file);
		return ;
	}
	
	new_file->next = g_temp_files;
	g_temp_files = new_file;
}

/* Remove temporary file from the list */
void	remove_temp_file(const char *filename)
{
	t_temp_file	*current;
	t_temp_file	*prev;
	
	if (!filename)
		return ;
	
	current = g_temp_files;
	prev = NULL;
	
	while (current)
	{
		if (ft_strcmp(current->filename, filename) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				g_temp_files = current->next;
			
			free(current->filename);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

/* Cleanup all temporary files */
void	cleanup_all_temp_files(void)
{
	t_temp_file	*current;
	t_temp_file	*next;
	
	current = g_temp_files;
	while (current)
	{
		next = current->next;
		unlink(current->filename);
		free(current->filename);
		free(current);
		current = next;
	}
	g_temp_files = NULL;
}

/* Initialize temporary file cleanup system */
void	init_temp_file_cleanup(void)
{
	atexit(cleanup_all_temp_files);
}

/* Cleanup temporary file */
void	cleanup_temp_file(const char *temp_filename)
{
	if (temp_filename)
	{
		unlink(temp_filename);
		remove_temp_file(temp_filename);
	}
}

/* Handle heredoc redirection (<<) using temporary file */
int	handle_heredoc_redirect(const char *delimiter)
{
	char	*temp_filename;
	int		result;

	if (!delimiter)
		return (-1);
	
	/* Create temporary file */
	temp_filename = create_temp_file();
	if (!temp_filename)
	{
		print_redirect_error("heredoc", "temporary file creation failed");
		return (-1);
	}

	/* Write heredoc content to temporary file */
	result = write_heredoc_to_temp(delimiter, temp_filename);
	if (result == -1)
	{
		cleanup_temp_file(temp_filename);
		free(temp_filename);
		return (-1);
	}

	/* Redirect stdin from temporary file */
	result = handle_input_redirect(temp_filename);
	
	/* Clean up temporary file */
	cleanup_temp_file(temp_filename);
	free(temp_filename);
	
	return (result);
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