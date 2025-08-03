/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_wait.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 02:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 02:56:39 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_cmd.h"
#include "signal_handler.h"
#include <errno.h>
#include <string.h>

static int	_helper_wait_for_pipeline(pid_t left_pid, pid_t right_pid)
{
	int	right_status;

	setup_parent_signals();
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &right_status, 0);
	set_signal();
	if (WIFEXITED(right_status))
		return (WEXITSTATUS(right_status));
	return (1);
}

static int	_helper_create_pipe_for_execution(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		ft_dprintf(STDERR_FILENO, ERROR_PREFIX "pipe failed: %s\n",
			strerror(errno));
		return (-1);
	}
	return (0);
}

int	execute_pipeline(t_command *left, t_command *right)
{
	int					pipe_fd[2];
	pid_t				left_pid;
	pid_t				right_pid;
	int					result;
	t_pipeline_params	params;

	if (!left || !right)
		return (1);
	if (_helper_create_pipe_for_execution(pipe_fd) == -1)
		return (1);
	params.pipe_fd = pipe_fd;
	params.left_pid = &left_pid;
	params.right_pid = &right_pid;
	result = execute_pipeline_process(left, right, &params);
	if (result != 0)
		return (result);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (_helper_wait_for_pipeline(left_pid, right_pid));
}
