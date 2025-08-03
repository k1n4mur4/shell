/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 03:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:47:01 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	remove_env_node(t_env *to_remove, t_env *prev)
{
	if (prev)
		prev->next = to_remove->next;
	else
		env(to_remove->next, SET);
	free(to_remove->key);
	free(to_remove->value);
	free(to_remove);
}

int	find_and_remove_env(const char *key)
{
	t_env	*env_list;
	t_env	*prev;

	env_list = env(NULL, GET);
	prev = NULL;
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
		{
			remove_env_node(env_list, prev);
			return (1);
		}
		prev = env_list;
		env_list = env_list->next;
	}
	return (0);
}
