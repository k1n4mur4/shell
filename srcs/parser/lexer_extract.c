/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extract.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:38 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 20:52:08 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "parser.h"

char	*extract_quoted_string(const char **input, char quote)
{
	const char	*start;
	int			len;
	char		*result;

	start = *input;
	(*input)++;
	while (**input && **input != quote)
		(*input)++;
	if (**input != quote)
		return (NULL);
	(*input)++;
	len = *input - start;
	result = ft_calloc(len + 1, sizeof(char));
	if (!result)
		return (NULL);
	ft_strlcpy(result, start, len + 1);
	return (result);
}

char	*extract_operator(const char **input)
{
	const char	*start;
	char		*result;
	int			len;

	start = *input;
	if ((**input == '<' || **input == '>') && *(*input + 1) == **input)
		*input += 2;
	else if (**input == '&' && *(*input + 1) == '&')
		*input += 2;
	else if (**input == '|' && *(*input + 1) == '|')
		*input += 2;
	else
		(*input)++;
	len = *input - start;
	result = ft_calloc(len + 1, sizeof(char));
	if (!result)
		return (NULL);
	ft_strlcpy(result, start, len + 1);
	return (result);
}

char	*extract_word(const char **input)
{
	const char	*start;
	char		*result;
	int			len;

	start = *input;
	while (**input && !is_whitespace(**input)
		&& !is_operator_char(**input) && **input != '\'' && **input != '"')
		(*input)++;
	len = *input - start;
	result = ft_calloc(len + 1, sizeof(char));
	if (!result)
		return (NULL);
	ft_strlcpy(result, start, len + 1);
	return (result);
}

char	*init_word_result(void)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	return (result);
}

char	*extract_complete_word(const char **input)
{
	char	*result;

	result = init_word_result();
	if (!result)
		return (NULL);
	while (**input && is_word_char(**input))
	{
		result = process_word_part(input, result);
		if (!result)
			return (NULL);
	}
	if (result && result[0] == '\0')
	{
		free(result);
		return (NULL);
	}
	return (result);
}
