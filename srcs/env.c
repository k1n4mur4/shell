/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:54 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 20:58:01 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "memory_utils.h"

void	dispose_env(t_env *env_list)
{
	cleanup_env_list(env_list);
}

t_env	*search_env(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	add_env_value(t_env *env, t_env *lst)
{
	char	*value;

	if (!env)
		return ;
	value = ft_strdup(lst->value);
	free(env->value);
	dispose_env(lst);
	env->value = value;
}

void	add_env_list(t_env *env, t_env *env_list)
{
	t_env	*lst;
	t_env	*last;
	t_env	*add_lst;

	last = env;
	if (env)
		while (last->next)
			last = last->next;
	while (env_list)
	{
		lst = env_list;
		env_list = env_list->next;
		lst->next = NULL;
		add_lst = search_env(env, lst->key);
		if (!add_lst)
		{
			if (last)
				last->next = lst;
			else
				env = lst;
			last = lst;
		}
		else
			add_env_value(add_lst, lst);
	}
}

t_env	*env(t_env *env_list, enum e_type type)
{
	static t_env	*env = NULL;

	if (type == GET)
		return (env);
	if (type == SET)
		env = env_list;
	if (type == ADD)
		add_env_list(env, env_list);
	if (type == SHOW)
		show_env_list(env);
	if (type == FREE)
		dispose_env(env);
	return (env);
}
