/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:47 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:53:31 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "parser.h"

static void	initialize_command_structure(t_command *command)
{
	command->type = CM_SIMPLE;
	command->simple = NULL;
	command->left = NULL;
	command->right = NULL;
}

static void	process_parsed_ast(t_command *command, t_command *ast)
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

void	parser(t_command *command)
{
	t_word_list	*word_list;
	t_command	*ast;

	if (!command || !command->current_command)
		return ;
	initialize_command_structure(command);
	word_list = parse_command_line(command->current_command);
	if (!word_list)
		return ;
	ast = parse_tokens_to_ast(word_list);
	if (ast)
		process_parsed_ast(command, ast);
	dispose_words(word_list);
}
