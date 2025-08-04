/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_word_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:44 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 20:50:49 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "make_cmd.h"

t_word_desc	*alloc_word_desc(void)
{
	t_word_desc	*temp;

	temp = (t_word_desc *)ft_calloc(1, sizeof(t_word_desc));
	if (!temp)
		return (NULL);
	temp->flags = 0;
	temp->word = NULL;
	return (temp);
}

t_word_desc	*make_bare_word(const char *string)
{
	t_word_desc	*temp;

	temp = alloc_word_desc();
	if (!temp)
		return (NULL);
	if (!string)
		return (temp);
	else
		temp->word = ft_strdup(string);
	return (temp);
}

static void	analyze_word_chars(t_word_desc *w, const char *string)
{
	size_t	i;
	size_t	slen;

	i = 0;
	slen = ft_strlen(string);
	while (i < slen)
	{
		if (string[i] == '$')
			w->flags |= W_HASDOLLAR;
		if (string[i] == '\'')
			w->flags |= W_QUOTED | W_SINGLEQUOTED;
		else if (string[i] == '"')
			w->flags |= W_QUOTED | W_DOUBLEQUOTED;
		else if (string[i] == '`')
			w->flags |= W_QUOTED;
		i++;
	}
}

t_word_desc	*make_word_flags(t_word_desc *w, const char *string)
{
	if (ft_strcmp(string, "||") == 0)
		w->flags |= W_OR;
	else if (ft_strcmp(string, "&&") == 0)
		w->flags |= W_AND;
	else if (ft_strcmp(string, "|") == 0)
		w->flags |= W_PIPE;
	else
		analyze_word_chars(w, string);
	return (w);
}
