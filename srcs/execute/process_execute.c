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
#include "process_execute_helpers.h"

static int	_helper_prepare_arrays(char **argv, char ***envp)
{
	if (!argv)
		return (1);
	*envp = create_envp_array();
	if (!*envp)
	{
		free_string_array(argv);
		return (1);
	}
	return (0);
}

static int	_helper_handle_fork_error(char **argv, char **envp)
{
	ft_dprintf(STDERR_FILENO, ERROR_PREFIX "fork failed: %s\n",
		strerror(errno));
	free_string_array(argv);
	free_string_array(envp);
	return (1);
}

static void	_helper_child_process(const char *command_path,
		const char *command_name, char **argv, char **envp)
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

static int	_helper_parent_process(pid_t pid, char **argv, char **envp)
{
	int	exit_code;

	setup_parent_signals();
	exit_code = wait_for_child_process(pid);
	set_signal();
	free_string_array(argv);
	free_string_array(envp);
	return (exit_code);
}

int	execute_external_command(const char *command_path,
		const char *command_name, t_word_list *args)
{
	pid_t	pid;
	char	**argv;
	char	**envp;

	if (!command_path)
		return (127);
	argv = helper_create_argv(command_path, command_name, args);
	if (_helper_prepare_arrays(argv, &envp) != 0)
		return (1);
	pid = fork();
	if (pid == -1)
		return (_helper_handle_fork_error(argv, envp));
	else if (pid == 0)
		_helper_child_process(command_path, command_name, argv, envp);
	else
		return (_helper_parent_process(pid, argv, envp));
	return (1);
}
