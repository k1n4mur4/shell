/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispose_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:26 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 20:55:06 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "make_cmd.h"

void	dispose_word(t_word_desc *w)
{
	if (!w)
		return ;
	if (w->word)
		free(w->word);
	free(w);
}

void	dispose_words(t_word_list *list)
{
	t_word_list	*temp;

	while (list)
	{
		temp = list;
		list = list->next;
		dispose_word(temp->word);
		free(temp);
	}
}

static t_word_desc	*create_word_copy(t_word_desc *source)
{
	t_word_desc	*new_word;

	new_word = make_word(source->word);
	if (!new_word)
		return (NULL);
	new_word->flags = source->flags;
	return (new_word);
}

static int	add_word_to_list(t_word_list **new_list, t_word_desc *new_word)
{
	t_word_list	*new_node;

	new_node = make_word_list(new_word, *new_list);
	if (!new_node)
		return (0);
	if (!*new_list)
		*new_list = new_node;
	return (1);
}

t_word_list	*copy_word_list(t_word_list *list)
{
	t_word_list	*new_list;
	t_word_list	*current;
	t_word_desc	*new_word;

	new_list = NULL;
	current = list;
	while (current)
	{
		new_word = create_word_copy(current->word);
		if (!new_word)
		{
			dispose_words(new_list);
			return (NULL);
		}
		if (!add_word_to_list(&new_list, new_word))
		{
			dispose_word(new_word);
			dispose_words(new_list);
			return (NULL);
		}
		current = current->next;
	}
	return (new_list);
}
