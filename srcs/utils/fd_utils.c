/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:44 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 20:48:40 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/fd_utils.h"
#include "ft_printf.h"
#include <errno.h>
#include <string.h>

int	setup_file_redirect(const char *filename, int target_fd, int flags)
{
	int	file_fd;

	if (!filename)
		return (-1);
	file_fd = open(filename, flags, 0644);
	if (file_fd == -1)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: %s: %s\n", filename, strerror(errno));
		return (-1);
	}
	if (dup2(file_fd, target_fd) == -1)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: dup2 failed: %s\n", strerror(errno));
		close(file_fd);
		return (-1);
	}
	close(file_fd);
	return (0);
}

int	backup_fd(int fd)
{
	int	backup_fd;

	backup_fd = dup(fd);
	if (backup_fd == -1)
		ft_dprintf(STDERR_FILENO,
			"minishell: backup fd failed: %s\n", strerror(errno));
	return (backup_fd);
}

int	restore_fd(int fd, int backup_fd)
{
	if (backup_fd == -1)
		return (-1);
	if (dup2(backup_fd, fd) == -1)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: restore fd failed: %s\n", strerror(errno));
		return (-1);
	}
	close(backup_fd);
	return (0);
}

void	close_if_valid(int fd)
{
	if (fd != -1)
		close(fd);
}

int	setup_input_redirect(const char *filename)
{
	return (setup_file_redirect(filename, STDIN_FILENO, O_RDONLY));
}
