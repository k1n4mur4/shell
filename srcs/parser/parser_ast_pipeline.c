/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 04:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "parser.h"
#include "parser_ast_helpers.h"

static t_command	*process_pipeline_element(t_command *left,
		t_word_list **tokens)
{
	t_command	*right;

	*tokens = (*tokens)->next;
	if (!*tokens)
	{
		parser_error(NULL, ERROR_PARSE, "\\n");
		dispose_ast_command(left);
		return (NULL);
	}
	right = parse_simple_command(tokens);
	if (!right)
	{
		dispose_ast_command(left);
		return (NULL);
	}
	left = make_binary_command(CM_PIPE, left, right);
	if (!left)
		dispose_ast_command(right);
	return (left);
}

t_command	*parse_pipeline(t_word_list **tokens)
{
	t_command	*left;

	left = parse_simple_command(tokens);
	if (!left)
		return (NULL);
	while (*tokens && ((*tokens)->word->flags & W_PIPE))
	{
		left = process_pipeline_element(left, tokens);
		if (!left)
			return (NULL);
	}
	return (left);
}
