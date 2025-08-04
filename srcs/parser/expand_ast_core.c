/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_ast_core.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 04:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "eval.h"
#include "make_cmd.h"
#include "parser.h"

char	*expand_word(const char *word)
{
	int	has_single;
	int	has_double;

	if (!word)
		return (NULL);
	if (ft_strchr(word, '\'') != NULL)
		has_single = 1;
	else
		has_single = 0;
	if (ft_strchr(word, '"') != NULL)
		has_double = 1;
	else
		has_double = 0;
	if (has_single && has_double)
		return (expand_mixed_quotes(word));
	else if (has_single)
		return (expand_single_quoted(word));
	else if (has_double)
		return (expand_double_quoted(word));
	else
		return (expand_unquoted(word));
}

char	*expand_word_with_flags(const char *word, int flags)
{
	if (!word)
		return (NULL);
	if (flags & W_SINGLEQUOTED)
		return (ft_strdup(word));
	else if (flags & W_DOUBLEQUOTED)
	{
		if (ft_strchr(word, '$'))
			return (expand_unquoted(word));
		else
			return (ft_strdup(word));
	}
	else if (flags & W_QUOTED)
	{
		if (ft_strchr(word, '$'))
			return (expand_unquoted(word));
		else
			return (ft_strdup(word));
	}
	return (expand_word(word));
}

int	should_expand_in_context(const char *word)
{
	if (!word)
		return (0);
	if (ft_strchr(word, '\'') != NULL)
		return (1);
	if (ft_strchr(word, '"') != NULL)
		return (1);
	if (ft_strchr(word, '$') != NULL)
		return (1);
	return (0);
}
