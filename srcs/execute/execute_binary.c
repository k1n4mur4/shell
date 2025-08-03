/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_binary.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 02:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 02:55:48 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_cmd.h"
#include "parser.h"

static int	_helper_execute_right_command(t_command *right)
{
	if (right)
	{
		expand_ast(right);
		execute_command(right);
		return (exit_value(0, GET));
	}
	return (exit_value(0, GET));
}

static int	_helper_handle_and_operation(t_command *right, int left_exit)
{
	if (left_exit == 0)
		return (_helper_execute_right_command(right));
	return (left_exit);
}

static int	_helper_handle_or_operation(t_command *right, int left_exit)
{
	if (left_exit != 0)
		return (_helper_execute_right_command(right));
	return (left_exit);
}

int	execute_binary_command(t_command *left, t_command *right,
		t_command_type type)
{
	int	left_exit;

	if (!left)
	{
		ft_dprintf(STDERR_FILENO,
			ERROR_PREFIX "binary command: missing left operand\n");
		return (1);
	}
	if (type != CM_AND && type != CM_OR)
	{
		ft_dprintf(STDERR_FILENO,
			ERROR_PREFIX "binary command: invalid operator type\n");
		return (1);
	}
	execute_command(left);
	left_exit = exit_value(0, GET);
	if (type == CM_AND)
		return (_helper_handle_and_operation(right, left_exit));
	else
		return (_helper_handle_or_operation(right, left_exit));
}
