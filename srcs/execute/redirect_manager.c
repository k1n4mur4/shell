/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:23 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:47:31 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect_utils.h"
#include <errno.h>

void	init_redirect_backup(t_redirect_backup *backup)
{
	backup->stdin_backup = -1;
	backup->stdout_backup = -1;
	backup->stderr_backup = -1;
}

static t_redirect	*find_last_redirect(t_redirect *redirects, int types)
{
	t_redirect	*current;
	t_redirect	*last_found;

	current = redirects;
	last_found = NULL;
	while (current)
	{
		if ((types & (1 << current->type)) != 0)
			last_found = current;
		current = current->next;
	}
	return (last_found);
}

int	setup_redirections(t_redirect *redirects, t_redirect_backup *backup)
{
	t_redirect	*last_input;
	t_redirect	*last_output;
	t_redirect	*last_heredoc;

	if (!backup)
		return (-1);
	backup->stdin_backup = dup(STDIN_FILENO);
	backup->stdout_backup = dup(STDOUT_FILENO);
	backup->stderr_backup = dup(STDERR_FILENO);
	if (backup->stdin_backup == -1 || backup->stdout_backup == -1
		|| backup->stderr_backup == -1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: failed to backup file descriptors: %s\n", strerror(errno));
		if (backup->stdin_backup != -1)
			close(backup->stdin_backup);
		if (backup->stdout_backup != -1)
			close(backup->stdout_backup);
		if (backup->stderr_backup != -1)
			close(backup->stderr_backup);
		init_redirect_backup(backup);
		return (-1);
	}
	last_input = find_last_redirect(redirects, (1 << R_INPUT));
	last_output = find_last_redirect(redirects, (1 << R_OUTPUT) | (1 << R_APPEND));
	last_heredoc = find_last_redirect(redirects, (1 << R_HEREDOC));
	if (last_heredoc)
	{
		if (handle_heredoc_redirect(last_heredoc->filename) == -1)
			return (-1);
	}
	else if (last_input)
	{
		if (handle_input_redirect(last_input->filename) == -1)
			return (-1);
	}
	if (last_output)
	{
		if (last_output->type == R_OUTPUT)
		{
			if (handle_output_redirect(last_output->filename) == -1)
				return (-1);
		}
		else if (last_output->type == R_APPEND)
		{
			if (handle_append_redirect(last_output->filename) == -1)
				return (-1);
		}
	}
	return (0);
}

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