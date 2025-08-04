/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:32 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "eval.h"
#include "expand_quotes_helpers.h"
#include "parser.h"

char	*expand_single_quoted(const char *word)
{
	int		len;
	char	*result;
	int		i;
	int		j;

	if (!word)
		return (NULL);
	len = ft_strlen(word);
	result = ft_calloc(len + 1, sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (word[i])
	{
		if (word[i] != '\'')
			result[j++] = word[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

char	*expand_double_quoted(const char *word)
{
	char	*result;
	int		i;
	int		prev_i;

	if (!word)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (word[i])
	{
		prev_i = i;
		result = process_double_quoted_char(result, word, &i);
		if (!result)
			return (NULL);
		if (i == prev_i)
		{
			free(result);
			return (NULL);
		}
	}
	return (result);
}

char	*expand_unquoted(const char *word)
{
	char	*result;
	int		i;
	int		prev_i;

	if (!word)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (word[i])
	{
		prev_i = i;
		result = process_unquoted_char(result, word, &i);
		if (!result)
			return (NULL);
		if (i == prev_i)
		{
			free(result);
			return (NULL);
		}
	}
	return (result);
}

static char	*process_quote_segment(const char *word, int *i)
{
	if (word[*i] == '\'')
		return (process_single_quote(word, i));
	else if (word[*i] == '"')
		return (process_double_quote(word, i));
	else
		return (process_unquoted(word, i));
}

char	*expand_mixed_quotes(const char *word)
{
	char	*result;
	char	*temp;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (word[i])
	{
		temp = process_quote_segment(word, &i);
		if (!temp)
		{
			free(result);
			return (NULL);
		}
		result = append_to_result(result, temp);
		if (!result)
			return (NULL);
	}
	return (result);
}
