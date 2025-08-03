/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_sorting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 03:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:29:21 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "memory_utils.h"

int	compare_env_keys(t_env *a, t_env *b)
{
	if (!a || !a->key)
		return (1);
	if (!b || !b->key)
		return (-1);
	return (ft_strcmp(a->key, b->key));
}

static void	swap_nodes(t_env **sorted, t_env *prev, t_env *current, t_env *next)
{
	if (prev)
		prev->next = next;
	else
		*sorted = next;
	current->next = next->next;
	next->next = current;
}

static int	bubble_sort_pass(t_env **sorted)
{
	t_env	*current;
	t_env	*next;
	t_env	*prev;
	int		swapped;

	swapped = 0;
	current = *sorted;
	prev = NULL;
	while (current && current->next)
	{
		next = current->next;
		if (compare_env_keys(current, next) > 0)
		{
			swap_nodes(sorted, prev, current, next);
			swapped = 1;
			prev = next;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
	return (swapped);
}

t_env	*sort_env_list(t_env *env_list)
{
	t_env	*sorted;
	int		swapped;

	if (!env_list || !env_list->next)
		return (env_list);
	sorted = env_list;
	swapped = 1;
	while (swapped)
		swapped = bubble_sort_pass(&sorted);
	return (sorted);
}
