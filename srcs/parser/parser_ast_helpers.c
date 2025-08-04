/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 04:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 15:48:58 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "error.h"

t_command	*handle_missing_right_operand(t_command *left)
{
	parser_error(NULL, ERROR_PARSE, "\\n");
	dispose_ast_command(left);
	return (NULL);
}

t_command	*create_binary_command(t_command *left, t_command *right,
		t_command_type type)
{
	t_command	*binary_cmd;

	binary_cmd = make_binary_command(type, left, right);
	if (!binary_cmd)
	{
		dispose_ast_command(left);
		dispose_ast_command(right);
		return (NULL);
	}
	return (binary_cmd);
}

t_redirect_type	get_redirect_type(const char *op)
{
	if (ft_strcmp(op, "<") == 0)
		return (R_INPUT);
	else if (ft_strcmp(op, ">") == 0)
		return (R_OUTPUT);
	else if (ft_strcmp(op, ">>") == 0)
		return (R_APPEND);
	else if (ft_strcmp(op, "<<") == 0)
		return (R_HEREDOC);
	return (R_INPUT);
}

int	validate_redirect_target(t_word_list **tokens, const char *op)
{
	if (!*tokens || !(*tokens)->word || !(*tokens)->word->word)
	{
		if (ft_strcmp(op, "<<") == 0)
			parser_error(NULL, ERROR_PARSE, "<<\\n");
		else if (ft_strcmp(op, ">>") == 0)
			parser_error(NULL, ERROR_PARSE, ">>\\n");
		else if (ft_strcmp(op, ">") == 0)
			parser_error(NULL, ERROR_PARSE, ">\\n");
		else
			parser_error(NULL, ERROR_PARSE, "<\\n");
		return (0);
	}
	return (1);
}

int	process_redirect(t_word_list **tokens, t_redirect **redirects)
{
	t_redirect		*new_redirect;
	t_redirect_type	type;
	const char		*op;

	op = (*tokens)->word->word;
	type = get_redirect_type(op);
	*tokens = (*tokens)->next;
	if (!validate_redirect_target(tokens, op))
		return (0);
	new_redirect = make_redirect(type, (*tokens)->word->word);
	if (!new_redirect)
		return (0);
	append_redirect(redirects, new_redirect);
	*tokens = (*tokens)->next;
	return (1);
}
