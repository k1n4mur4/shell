/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:49:39 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:52:54 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exit_value.h"
#include "make_cmd.h"
#include "minishell.h"
#include "redirect_utils.h"
#include "shell_helpers.h"
#include "signal_handler.h"
#include <unistd.h>

static int	execute_stdin_commands(void)
{
	char	*full_input;
	int		last_exit_code;

	full_input = read_all_input();
	if (full_input)
	{
		last_exit_code = process_input(full_input);
		free(full_input);
		return (last_exit_code);
	}
	return (0);
}

static int	handle_command_option(char **argv, char **envp)
{
	int	result;

	initialize_enviroment(envp);
	shell_initialize();
	result = execute_command_string(argv[2]);
	exit_shell();
	return (result);
}

int	shell(int argc, char **argv, char **envp)
{
	if (argc >= 3 && ft_strcmp(argv[1], "-c") == 0)
		return (handle_command_option(argv, envp));
	if (argc > 1)
	{
		ft_dprintf(STDERR_FILENO,
			"%s: %s: No such file or directory\n",
			ENAME, argv[1]);
		return (127);
	}
	initialize_enviroment(envp);
	shell_initialize();
	if (isatty(STDIN_FILENO))
		reader_loop();
	else
		execute_stdin_commands();
	exit_shell();
	return (exit_value(0, GET));
}
