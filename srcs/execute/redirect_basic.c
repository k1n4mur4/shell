/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_basic.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:17 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 01:48:19 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect_utils.h"
#include <sys/stat.h>

void	print_redirect_error(const char *filename, const char *error_msg)
{
	ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", filename, error_msg);
}

int	handle_input_redirect(const char *filename)
{
	int			fd;
	struct stat	file_stat;

	if (!filename)
		return (-1);
	if (stat(filename, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
	{
		print_redirect_error(filename, "Is a directory");
		return (-1);
	}
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
	int			fd;
	struct stat	file_stat;

	if (!filename)
		return (-1);
	if (stat(filename, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
	{
		print_redirect_error(filename, "Is a directory");
		return (-1);
	}
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
	int fd;
	struct stat file_stat;

	if (!filename)
		return (-1);

	if (stat(filename, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
	{
		print_redirect_error(filename, "Is a directory");
		return (-1);
	}

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