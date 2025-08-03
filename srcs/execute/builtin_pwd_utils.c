/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 03:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:51:13 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	update_existing_env(t_env *node, char *value)
{
	free(node->value);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
}

void	create_new_env(const char *key, char *value)
{
	t_env	*new_node;

	if (!value)
		return ;
	new_node = ft_calloc(1, sizeof(t_env));
	if (new_node)
	{
		new_node->key = ft_strdup(key);
		new_node->value = ft_strdup(value);
		new_node->next = NULL;
		env(new_node, ADD);
	}
}

void	update_oldpwd_var(char *old_pwd)
{
	t_env	*env_list;
	t_env	*oldpwd_node;

	env_list = env(NULL, GET);
	oldpwd_node = search_env(env_list, "OLDPWD");
	if (oldpwd_node)
		update_existing_env(oldpwd_node, old_pwd);
	else
		create_new_env("OLDPWD", old_pwd);
}

void	update_pwd_var(char *new_pwd)
{
	t_env	*env_list;
	t_env	*pwd_node;

	env_list = env(NULL, GET);
	pwd_node = search_env(env_list, "PWD");
	if (pwd_node)
		update_existing_env(pwd_node, new_pwd);
	else
		create_new_env("PWD", new_pwd);
}

void	update_pwd_vars(char *old_pwd, char *new_pwd)
{
	update_oldpwd_var(old_pwd);
	update_pwd_var(new_pwd);
}
