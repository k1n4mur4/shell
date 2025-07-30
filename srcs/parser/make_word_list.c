/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_word_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:44 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:38:02 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "make_cmd.h"

t_word_desc	*make_word(const char *string)
{
	t_word_desc	*word_desc;

	if (!string)
		return (NULL);
	word_desc = make_bare_word(string);
	if (!word_desc)
		return (NULL);
	return (make_word_flags(word_desc, string));
}

t_word_desc	*copy_word_desc(const t_word_desc *original)
{
	t_word_desc	*copy;

	if (!original)
		return (NULL);
	copy = alloc_word_desc();
	if (!copy)
		return (NULL);
	if (original->word)
	{
		copy->word = ft_strdup(original->word);
		if (!copy->word)
		{
			free(copy);
			return (NULL);
		}
	}
	copy->flags = original->flags;
	return (copy);
}

t_word_list	*make_word_list(t_word_desc *word, t_word_list *wlink)
{
	t_word_list	*temp;
	t_word_list	*last;

	temp = (t_word_list *)ft_calloc(1, sizeof(t_word_list));
	if (!temp)
		return (NULL);
	temp->word = word;
	if (wlink)
	{
		last = wlink;
		while (last->next)
			last = last->next;
		last->next = temp;
		temp = wlink;
	}
	return (temp);
}