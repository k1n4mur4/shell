/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_value.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:49:27 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 01:49:27 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exit_value.h"

int	exit_value(int exit_value, enum e_type type)
{
	static int	last_command_exit_value;

	last_command_exit_value = 0;
	if (type == SET)
	{
		/* Ensure exit value is in valid range (0-255) for bash compatibility */
		last_command_exit_value = exit_value & 255;
	}
	else if (type == GET)
	{
		exit_value = last_command_exit_value;
	}
	return (exit_value);
}

int	shell_exit_status(int exit_code, enum e_type type)
{
	static int	exit_code_value;
	static int	should_exit;

	exit_code_value = 0;
	should_exit = 0;
	if (type == SET)
	{
		exit_code_value = exit_code & 255;
		should_exit = 1;
	}
	else if (type == GET)
	{
		if (should_exit)
		{
			should_exit = 0;
			return (exit_code_value);
		}
		return (-1);
	}
	return (exit_code_value);
}
