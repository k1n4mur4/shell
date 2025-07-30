/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:47 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:39:24 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "parser.h"

t_command	*parse_and_or(t_word_list **tokens)
{
	t_command	*left;
	t_command	*right;
	int			is_and;

	left = parse_pipeline(tokens);
	if (!left)
		return (NULL);
	while (*tokens && ((*tokens)->word->flags & (W_AND | W_OR)))
	{
		is_and = ((*tokens)->word->flags & W_AND);
		*tokens = (*tokens)->next;
		if (!*tokens)
		{
			parser_error(NULL, ERROR_PARSE, "\\n");
			dispose_ast_command(left);
			return (NULL);
		}
		right = parse_pipeline(tokens);
		if (!right)
		{
			dispose_ast_command(left);
			return (NULL);
		}
		if (is_and)
			left = make_binary_command(CM_AND, left, right);
		else
			left = make_binary_command(CM_OR, left, right);
		if (!left)
		{
			dispose_ast_command(right);
			return (NULL);
		}
	}
	return (left);
}

t_command	*parse_pipeline(t_word_list **tokens)
{
	t_command	*left;
	t_command	*right;

	left = parse_simple_command(tokens);
	if (!left)
		return (NULL);
	while (*tokens && ((*tokens)->word->flags & W_PIPE))
	{
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
		{
			dispose_ast_command(right);
			return (NULL);
		}
	}
	return (left);
}

t_command	*parse_simple_command(t_word_list **tokens)
{
	t_word_list		*words;
	t_redirect		*redirects;
	t_word_list		*current;
	t_redirect_type	type;
	t_redirect		*new_redirect;
	t_word_desc		*word_copy;

	if (!tokens || !*tokens)
		return (NULL);
	words = NULL;
	redirects = NULL;
	while (*tokens)
	{
		current = *tokens;
		if (current->word->flags & W_PIPE)
			break ;
		if (current->word->flags & (W_AND | W_OR))
			break ;
		if (is_redirect_operator(current->word->word))
		{
			if (ft_strcmp(current->word->word, ">>") == 0)
				type = R_APPEND;
			else if (ft_strcmp(current->word->word, "<<") == 0)
				type = R_HEREDOC;
			else if (ft_strcmp(current->word->word, "<") == 0)
				type = R_INPUT;
			else if (ft_strcmp(current->word->word, ">") == 0)
				type = R_OUTPUT;
			else
			{
				*tokens = (*tokens)->next;
				continue ;
			}
			*tokens = (*tokens)->next;
			if (!*tokens || !(*tokens)->word)
			{
				parser_error(NULL, ERROR_PARSE, "\\n");
				dispose_words(words);
				dispose_redirects(redirects);
				return (NULL);
			}
			if (is_redirect_operator((*tokens)->word->word) ||
				((*tokens)->word->flags & W_PIPE) ||
				((*tokens)->word->word && ft_strcmp((*tokens)->word->word,
							"&&") == 0))
			{
				parser_error(NULL, ERROR_PARSE, (*tokens)->word->word);
				dispose_words(words);
				dispose_redirects(redirects);
				return (NULL);
			}
			new_redirect = make_redirect(type, (*tokens)->word->word);
			if (new_redirect)
			{
				new_redirect->next = NULL;
				append_redirect(&redirects, new_redirect);
			}
			*tokens = (*tokens)->next;
		}
		else
		{
			word_copy = copy_word_desc(current->word);
			if (!word_copy)
			{
				dispose_words(words);
				dispose_redirects(redirects);
				return (NULL);
			}
			words = make_word_list(word_copy, words);
			if (!words)
			{
				dispose_word(word_copy);
				dispose_redirects(redirects);
				return (NULL);
			}
			*tokens = (*tokens)->next;
		}
	}
	return (make_simple_command(words, redirects));
}