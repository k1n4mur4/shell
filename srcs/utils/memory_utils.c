/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:44 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 20:47:05 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/memory_utils.h"
#include "env.h"

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_linked_list(void *head, void (*free_node)(void *))
{
	void	*current;
	void	*next;

	if (!head || !free_node)
		return ;
	current = head;
	while (current)
	{
		next = *(void **)current;
		free_node(current);
		current = next;
	}
}

void	free_env_node(void *node)
{
	t_env	*env_node;

	if (!node)
		return ;
	env_node = (t_env *)node;
	if (env_node->key)
		free(env_node->key);
	if (env_node->value)
		free(env_node->value);
	free(env_node);
}

void	cleanup_env_list(void *env_list)
{
	free_linked_list(env_list, free_env_node);
}
