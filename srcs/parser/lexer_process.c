/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:38 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 20:51:33 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "parser.h"

char	*process_word_part(const char **input, char *result)
{
	char	*part;
	char	*tmp;

	if (**input == '\'' || **input == '"')
		part = extract_quoted_string(input, **input);
	else
		part = extract_word(input);
	if (!part)
	{
		if (result[0] == '\0')
		{
			free(result);
			return (NULL);
		}
		return (result);
	}
	tmp = append_string(result, part);
	free(result);
	free(part);
	return (tmp);
}

char	*get_next_token(const char **input)
{
	lexer_skip_whitespace(input);
	if (!**input)
		return (NULL);
	if (is_operator_char(**input))
		return (extract_operator(input));
	else
		return (extract_complete_word(input));
}

static char	detect_quote_type(const char **current)
{
	lexer_skip_whitespace(current);
	if (**current == '\'' || **current == '"')
		return (**current);
	return (0);
}

static void	set_quote_flags(t_word_desc *word_desc, char quote_char)
{
	if (quote_char == '\'')
		word_desc->flags |= W_SINGLEQUOTED;
	else if (quote_char == '"')
		word_desc->flags |= W_DOUBLEQUOTED;
}

t_word_list	*process_single_token(const char **current,
											t_word_list *word_list)
{
	t_word_desc	*word_desc;
	char		*token;
	t_word_list	*new_word_list;
	char		quote_char;

	quote_char = detect_quote_type(current);
	token = get_next_token(current);
	if (!token)
		return (word_list);
	word_desc = make_word(token);
	set_quote_flags(word_desc, quote_char);
	free(token);
	if (!word_desc)
	{
		dispose_words(word_list);
		return (NULL);
	}
	new_word_list = make_word_list(word_desc, word_list);
	if (!new_word_list)
	{
		dispose_word(word_desc);
		dispose_words(word_list);
		return (NULL);
	}
	return (new_word_list);
}
