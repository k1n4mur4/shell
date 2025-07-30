/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispose_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:37:38 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:47:45 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dispose_cmd.h"
#include "make_cmd.h"

void	dispose_current_command(t_command *command)
{
	if (!command)
		return ;
	if (command->simple)
	{
		dispose_words(command->simple->words);
		dispose_redirects(command->simple->redirects);
		free(command->simple);
		command->simple = NULL;
	}
	if (command->left)
	{
		dispose_ast_command(command->left);
		command->left = NULL;
	}
	if (command->right)
	{
		dispose_ast_command(command->right);
		command->right = NULL;
	}
	if (command->current_command)
	{
		free(command->current_command);
		command->current_command = NULL;
	}
}
