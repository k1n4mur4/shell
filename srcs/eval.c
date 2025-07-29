/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:49:02 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 02:05:33 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "exit_value.h"
#include "make_cmd.h"
#include "signal_handler.h"

int	reader_loop(void)
{
	t_command	command;

	set_signal();
	while (1)
	{
		command.current_command = readline(PROMPT);
		if (!command.current_command)
		{
			write(2, "exit\n", 5);
			break ;
		}
		if (!*(command.current_command))
		{
			free(command.current_command);
			continue ;
		}
		set_deftext(command.current_command);
		parser(&command);
		execute_command(&command);
		dispose_current_command(&command);
		if (shell_exit_status(0, GET) >= 0)
			return (shell_exit_status(0, GET));
	}
	return (exit_value(0, GET));
}
