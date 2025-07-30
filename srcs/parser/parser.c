/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:47 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:39:24 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "parser.h"

void	parser(t_command *command)
{
	t_word_list *word_list;
	t_command *ast;

	if (!command || !command->current_command)
		return ;
	command->type = CM_SIMPLE;
	command->simple = NULL;
	command->left = NULL;
	command->right = NULL;
	word_list = parse_command_line(command->current_command);
	if (!word_list)
		return ;
	ast = parse_tokens_to_ast(word_list);
	if (ast)
	{
		expand_ast(ast);
		command->type = ast->type;
		command->simple = ast->simple;
		command->left = ast->left;
		command->right = ast->right;
		ast->simple = NULL;
		ast->left = NULL;
		ast->right = NULL;
		dispose_ast_command(ast);
	}
	dispose_words(word_list);
}

