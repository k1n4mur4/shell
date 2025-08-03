/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_command_execution.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 03:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:52:36 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exit_value.h"
#include "make_cmd.h"
#include "minishell.h"
#include "redirect_utils.h"
#include "shell_helpers.h"
#include "signal_handler.h"
#include <unistd.h>

int	execute_single_command(char *trimmed)
{
	t_command	command;

	command.type = CM_SIMPLE;
	command.simple = NULL;
	command.left = NULL;
	command.right = NULL;
	command.current_command = ft_strdup(trimmed);
	if (command.current_command)
	{
		parser(&command);
		execute_command(&command);
		dispose_current_command(&command);
		return (exit_value(0, GET));
	}
	return (0);
}

int	process_command_part(char *trimmed, char delimiter)
{
	if (delimiter == '\n')
		return (execute_command_parts(trimmed, ';'));
	else
		return (execute_single_command(trimmed));
}

static int	_helper_process_command_segment(char *part, char delimiter)
{
	char	*trimmed;
	int		exit_code;

	trimmed = (char *)skip_whitespace(part);
	if (*trimmed)
	{
		exit_code = process_command_part(trimmed, delimiter);
		if (shell_exit_status(0, GET) >= 0)
			return (shell_exit_status(0, GET));
		return (exit_code);
	}
	return (0);
}

static char	*_helper_find_and_split_delimiter(char *part, char delimiter)
{
	char	*next_delim;

	next_delim = ft_strchr(part, delimiter);
	if (next_delim)
		*next_delim = '\0';
	return (next_delim);
}

int	execute_command_parts(char *cmd_str, char delimiter)
{
	char	*part;
	char	*next_delim;
	int		last_exit_code;

	last_exit_code = 0;
	part = cmd_str;
	while (part && *part)
	{
		next_delim = _helper_find_and_split_delimiter(part, delimiter);
		last_exit_code = _helper_process_command_segment(part, delimiter);
		if (shell_exit_status(0, GET) >= 0)
			break ;
		if (next_delim)
			part = next_delim + 1;
		else
			break ;
	}
	return (last_exit_code);
}
