/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:47 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "parser.h"
#include "parser_ast_helpers.h"

static t_command	*process_and_or_operation(t_command *left,
		t_word_list **tokens)
{
	t_command	*right;
	int			is_and;

	is_and = ((*tokens)->word->flags & W_AND);
	*tokens = (*tokens)->next;
	if (!*tokens)
		return (handle_missing_right_operand(left));
	right = parse_pipeline(tokens);
	if (!right)
	{
		dispose_ast_command(left);
		return (NULL);
	}
	if (is_and)
		return (create_binary_command(left, right, CM_AND));
	else
		return (create_binary_command(left, right, CM_OR));
}

t_command	*parse_and_or(t_word_list **tokens)
{
	t_command	*left;

	left = parse_pipeline(tokens);
	if (!left)
		return (NULL);
	while (*tokens && ((*tokens)->word->flags & (W_AND | W_OR)))
	{
		left = process_and_or_operation(left, tokens);
		if (!left)
			return (NULL);
	}
	return (left);
}

static int	process_word(t_word_list **tokens, t_word_list **words,
		t_redirect **redirects)
{
	t_word_desc	*word_copy;

	word_copy = copy_word_desc((*tokens)->word);
	if (!word_copy)
	{
		dispose_words(*words);
		dispose_redirects(*redirects);
		return (0);
	}
	*words = make_word_list(word_copy, *words);
	if (!*words)
	{
		dispose_word(word_copy);
		dispose_redirects(*redirects);
		return (0);
	}
	*tokens = (*tokens)->next;
	return (1);
}

t_command	*parse_simple_command(t_word_list **tokens)
{
	t_word_list	*words;
	t_redirect	*redirects;

	if (!tokens || !*tokens)
		return (NULL);
	words = NULL;
	redirects = NULL;
	while (*tokens)
	{
		if ((*tokens)->word->flags & (W_PIPE | W_AND | W_OR))
			break ;
		if (is_redirect_operator((*tokens)->word->word))
		{
			if (!process_redirect(tokens, &redirects))
				return (NULL);
		}
		else
		{
			if (!process_word(tokens, &words, &redirects))
				return (NULL);
		}
	}
	return (make_simple_command(words, redirects));
}
