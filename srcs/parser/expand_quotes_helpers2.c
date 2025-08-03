/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes_helpers2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 04:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:06:27 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "expand_quotes_helpers.h"

char	*process_single_quote(const char *word, int *i)
{
	int		start;
	int		end;
	char	*result;

	start = *i + 1;
	end = start;
	while (word[end] && word[end] != '\'')
		end++;
	if (word[end] == '\'')
	{
		result = ft_substr(word, start, end - start);
		*i = end + 1;
		return (result);
	}
	*i = end;
	return (ft_strdup(""));
}

char	*process_double_quote(const char *word, int *i)
{
	char	*result;
	int		start;

	start = *i + 1;
	*i = start;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (word[*i] && word[*i] != '"')
	{
		result = process_double_quoted_char(result, word, i);
		if (!result)
			return (NULL);
	}
	if (word[*i] == '"')
		(*i)++;
	return (result);
}

char	*process_unquoted(const char *word, int *i)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (word[*i] && word[*i] != '\'' && word[*i] != '"')
	{
		result = process_unquoted_char(result, word, i);
		if (!result)
			return (NULL);
	}
	return (result);
}

char	*append_to_result(char *result, char *temp)
{
	char	*new_result;

	if (!temp)
		return (result);
	new_result = ft_strjoin(result, temp);
	free(result);
	free(temp);
	return (new_result);
}

int	process_word(t_word_list **tokens, t_word_list **words,
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
