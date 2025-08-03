/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 02:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:54:13 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_cmd.h"
#include "signal_handler.h"
#include <errno.h>
#include <signal.h>
#include <string.h>

static int	_helper_create_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		ft_dprintf(STDERR_FILENO, ERROR_PREFIX "pipe failed: %s\n",
			strerror(errno));
		return (-1);
	}
	return (0);
}

static void	_helper_left_child_process(t_command *left, int pipe_fd[2])
{
	setup_child_signals();
	close(pipe_fd[0]);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
	{
		ft_dprintf(STDERR_FILENO, ERROR_PREFIX "dup2 failed: %s\n",
			strerror(errno));
		close(pipe_fd[1]);
		exit(1);
	}
	close(pipe_fd[1]);
	execute_command(left);
	exit(exit_value(0, GET));
}

static void	_helper_right_child_process(t_command *right, int pipe_fd[2])
{
	setup_child_signals();
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		ft_dprintf(STDERR_FILENO, ERROR_PREFIX "dup2 failed: %s\n",
			strerror(errno));
		close(pipe_fd[0]);
		exit(1);
	}
	close(pipe_fd[0]);
	execute_command(right);
	exit(exit_value(0, GET));
}

static int	_helper_handle_fork_error(int pipe_fd[2], pid_t left_pid)
{
	ft_dprintf(STDERR_FILENO, ERROR_PREFIX "fork failed: %s\n",
		strerror(errno));
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	if (left_pid > 0)
	{
		kill(left_pid, SIGTERM);
		waitpid(left_pid, NULL, 0);
	}
	return (1);
}

int	execute_pipeline_process(t_command *left, t_command *right,
		t_pipeline_params *params)
{
	*(params->left_pid) = fork();
	if (*(params->left_pid) == -1)
		return (_helper_handle_fork_error(params->pipe_fd, 0));
	else if (*(params->left_pid) == 0)
		_helper_left_child_process(left, params->pipe_fd);
	*(params->right_pid) = fork();
	if (*(params->right_pid) == -1)
		return (_helper_handle_fork_error(params->pipe_fd,
				*(params->left_pid)));
	else if (*(params->right_pid) == 0)
		_helper_right_child_process(right, params->pipe_fd);
	return (0);
}
