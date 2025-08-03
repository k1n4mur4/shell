/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_enviroment.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:49:30 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:06:46 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "initialize_env_helpers.h"

t_env	*make_env_list(char *key, char *value)
{
	t_env	*temp;

	if (!key)
		return (NULL);
	temp = alloc_env();
	if (!temp)
		return (NULL);
	temp->key = key;
	temp->value = value;
	return (temp);
}

static char	*extract_env_key(char *envp, int *key_len)
{
	int	i;

	i = 0;
	while (envp[i] != '=' && envp[i] != '\0')
		i++;
	if (envp[i] == '\0')
		return (NULL);
	*key_len = i;
	return (ft_strndup(envp, i));
}

static char	*extract_env_value(char *envp, int key_len)
{
	return (ft_strdup(envp + key_len + 1));
}

t_env	*initialize_env(char *envp)
{
	t_env	*new;
	char	*key;
	char	*value;
	int		key_len;

	key = extract_env_key(envp, &key_len);
	if (!key)
		return (NULL);
	value = extract_env_value(envp, key_len);
	if (!value)
	{
		free(key);
		return (NULL);
	}
	new = make_env_list(key, value);
	if (!new)
	{
		free(key);
		free(value);
		return (NULL);
	}
	return (new);
}

void	initialize_enviroment(char **envp)
{
	int	count;

	if (!envp || !*envp)
	{
		return ;
	}
	count = 0;
	env(initialize_env(*envp), SET);
	envp++;
	count++;
	while (*envp)
	{
		env(initialize_env(*envp), ADD);
		envp++;
		count++;
	}
}
