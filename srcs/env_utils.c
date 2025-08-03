/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:54 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "env_sorting.h"
#include "memory_utils.h"

static t_env	*create_env_node(t_env *source)
{
	t_env	*new_node;

	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return (NULL);
	if (source->key)
		new_node->key = ft_strdup(source->key);
	if (source->value)
		new_node->value = ft_strdup(source->value);
	return (new_node);
}

static void	link_env_node(t_env **copy, t_env **last, t_env *new_node)
{
	if (!*copy)
		*copy = new_node;
	else
		(*last)->next = new_node;
	*last = new_node;
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
		new_node = create_env_node(current);
		if (!new_node)
			return (NULL);
		link_env_node(&copy, &last, new_node);
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
