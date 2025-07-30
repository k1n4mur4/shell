/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:38 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 20:51:41 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "parser.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}

void	lexer_skip_whitespace(const char **input)
{
	while (**input && is_whitespace(**input))
		(*input)++;
}

int	is_word_char(char c)
{
	return (!is_whitespace(c) && !is_operator_char(c));
}

char	*append_string(char *base, char *append)
{
	char	*new_str;
	size_t	base_len;
	size_t	append_len;
	size_t	new_len;

	if (!base || !append)
		return (NULL);
	base_len = ft_strlen(base);
	append_len = ft_strlen(append);
	new_len = base_len + append_len + 1;
	new_str = ft_calloc(new_len, sizeof(char));
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, base, new_len);
	ft_strlcat(new_str, append, new_len);
	return (new_str);
}
