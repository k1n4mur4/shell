/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 02:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:55:29 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_cmd.h"
#include "path_utils.h"
#include "process_utils.h"
#include <unistd.h>

static int	_helper_check_file_error(const char *command_name)
{
	if (access(command_name, F_OK) == -1)
	{
		ft_dprintf(STDERR_FILENO,
			ERROR_PREFIX "%s: No such file or directory\n",
			command_name);
		return (127);
	}
	else if (access(command_name, X_OK) == -1)
	{
		ft_dprintf(STDERR_FILENO,
			ERROR_PREFIX "%s: Permission denied\n",
			command_name);
		return (126);
	}
	else
	{
		ft_dprintf(STDERR_FILENO,
			ERROR_PREFIX "%s: command not found\n",
			command_name);
		return (127);
	}
}

static int	_helper_execute_external_with_path(char *command_path,
		const char *command_name, t_word_list *args)
{
	int	exit_code;

	exit_code = execute_external_command(command_path, command_name, args);
	free(command_path);
	return (exit_code);
}

static int	_helper_handle_command_not_found(const char *command_name)
{
	if (ft_strchr(command_name, '/'))
		return (_helper_check_file_error(command_name));
	else
	{
		ft_dprintf(STDERR_FILENO,
			ERROR_PREFIX "%s: command not found\n",
			command_name);
		return (127);
	}
}

int	execute_external_command_wrapper(char *command_name, t_word_list *args)
{
	char	*command_path;

	command_path = find_command_path(command_name);
	if (!command_path)
		return (_helper_handle_command_not_found(command_name));
	return (_helper_execute_external_with_path(command_path,
			command_name, args));
}
