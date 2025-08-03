/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:03 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 02:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_cmd.h"

void	execute_command(t_command *command)
{
	int	exit_code;

	if (!command)
		return ;
	exit_code = 0;
	if (command->type == CM_SIMPLE && command->simple)
		exit_code = execute_simple_command(command->simple);
	else if (command->type == CM_PIPE)
		exit_code = execute_pipeline(command->left, command->right);
	else if (command->type == CM_AND || command->type == CM_OR)
		exit_code = execute_binary_command(command->left, command->right,
				command->type);
	exit_value(exit_code, SET);
}
