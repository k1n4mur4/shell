/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_input_processing.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 03:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:29:07 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exit_value.h"
#include "make_cmd.h"
#include "minishell.h"
#include "redirect_utils.h"
#include "shell_helpers.h"
#include "signal_handler.h"
#include <unistd.h>

const char	*skip_whitespace(const char *str)
{
	while (str && *str && (*str == ' ' || *str == '\t' || *str == '\n'))
		str++;
	return (str);
}

int	execute_command_string(const char *command_str)
{
	const char	*trimmed;
	char		*cmd_copy;
	int			last_exit_code;

	if (!command_str)
		return (0);
	trimmed = skip_whitespace(command_str);
	if (!*trimmed)
		return (0);
	cmd_copy = ft_strdup(command_str);
	if (!cmd_copy)
		return (1);
	last_exit_code = execute_command_parts(cmd_copy, '\n');
	free(cmd_copy);
	return (last_exit_code);
}

static char	*_helper_append_input_line(char *full_input, char *line)
{
	char	*temp;

	if (full_input)
	{
		temp = ft_strjoin(full_input, line);
		free(full_input);
		return (temp);
	}
	else
		return (ft_strdup(line));
}

char	*read_all_input(void)
{
	char	*line;
	char	*full_input;
	size_t	len;

	line = NULL;
	full_input = NULL;
	len = 0;
	while (getline(&line, &len, stdin) != -1)
	{
		if (line && *line)
			full_input = _helper_append_input_line(full_input, line);
	}
	if (line)
		free(line);
	return (full_input);
}

int	process_input(char *full_input)
{
	int	result;
	int	last_exit_code;

	if (full_input[ft_strlen(full_input) - 1] == '\n')
		full_input[ft_strlen(full_input) - 1] = '\0';
	result = execute_command_string(full_input);
	if (shell_exit_status(0, GET) >= 0)
		last_exit_code = shell_exit_status(0, GET);
	else if (result != 0)
		last_exit_code = result;
	else
		last_exit_code = 0;
	return (last_exit_code);
}
