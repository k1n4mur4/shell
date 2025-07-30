/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:54 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 20:46:11 by kinamura         ###   ########.fr       */
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

t_env	*sort_env_list(t_env *env_list)
{
	t_env	*sorted;
	t_env	*current;
	t_env	*next;
	t_env	*prev;
	int		swapped;

	if (!env_list || !env_list->next)
		return (env_list);
	sorted = env_list;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = sorted;
		prev = NULL;
		while (current && current->next)
		{
			next = current->next;
			if (compare_env_keys(current, next) > 0)
			{
				if (prev)
					prev->next = next;
				else
					sorted = next;
				current->next = next->next;
				next->next = current;
				swapped = 1;
				prev = next;
			}
			else
			{
				prev = current;
				current = current->next;
			}
		}
	}
	return (sorted);
}

t_env	*copy_env_list(t_env *env_list)
{
	t_env	*copy;
	t_env	*new_node;
	t_env	*current;
	t_env	*last;

	if (!env_list)
		return (NULL);
	copy = NULL;
	last = NULL;
	current = env_list;
	while (current)
	{
		new_node = ft_calloc(1, sizeof(t_env));
		if (!new_node)
			return (NULL);
		if (current->key)
			new_node->key = ft_strdup(current->key);
		if (current->value)
			new_node->value = ft_strdup(current->value);
		if (!copy)
			copy = new_node;
		else
			last->next = new_node;
		last = new_node;
		current = current->next;
	}
	return (copy);
}

void	free_env_list(t_env *env_list)
{
	cleanup_env_list(env_list);
}

void	show_env_list(t_env *env_list)
{
	t_env	*sorted_copy;
	t_env	*current;

	sorted_copy = copy_env_list(env_list);
	if (!sorted_copy)
		return ;
	sorted_copy = sort_env_list(sorted_copy);
	current = sorted_copy;
	while (current)
	{
		if (current->key)
		{
			ft_dprintf(STDOUT_FILENO, "declare -x %s", current->key);
			if (current->value)
				ft_dprintf(STDOUT_FILENO, "=\"%s\"", current->value);
			ft_dprintf(STDOUT_FILENO, "\n");
		}
		current = current->next;
	}
	free_env_list(sorted_copy);
}
