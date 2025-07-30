/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_execute.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:12 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 21:08:55 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "process_utils.h"
#include "signal_handler.h"
#include "memory_utils.h"

int	wait_for_child_process(pid_t pid)
{
	int	status;
	int	exit_code;

	if (waitpid(pid, &status, 0) == -1)
	{
		ft_dprintf(STDERR_FILENO, ERROR_PREFIX "waitpid failed: %s\n",
				strerror(errno));
		return (1);
	}
	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		return (exit_code);
	}
	else if (WIFSIGNALED(status))
	{
		exit_code = 128 + WTERMSIG(status);
		return (exit_code);
	}
	return (1);
}

int	execute_external_command(const char *command_path, const char *command_name,
		t_word_list *args)
{
	pid_t	pid;
	char	**argv;
	char	**envp;
	int		exit_code;

	if (!command_path)
		return (127);
	argv = create_argv_array(command_path, command_name, args);
	if (!argv)
		return (1);
	envp = create_envp_array();
	if (!envp)
	{
		free_string_array(argv);
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		ft_dprintf(STDERR_FILENO, ERROR_PREFIX "fork failed: %s\n",
			strerror(errno));
		free_string_array(argv);
		free_string_array(envp);
		return (1);
	}
	else if (pid == 0)
	{
		setup_child_signals();
		if (execve(command_path, argv, envp) == -1)
		{
			ft_dprintf(STDERR_FILENO, ERROR_PREFIX "%s: %s\n", command_name,
				strerror(errno));
			free_string_array(argv);
			free_string_array(envp);
			if (errno == EACCES)
				exit(126);
			else
				exit(127);
		}
	}
	else
	{
		setup_parent_signals();
		exit_code = wait_for_child_process(pid);
		set_signal();
		free_string_array(argv);
		free_string_array(envp);
		return (exit_code);
	}
	return (1);
}
