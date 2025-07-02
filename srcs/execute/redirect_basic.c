#include "redirect_utils.h"

void	print_redirect_error(const char *filename, const char *error_msg)
{
	ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", filename, error_msg);
}

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