/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_management.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:47:54 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:39:43 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	builtin_env(t_word_list *args)
{
	t_env	*env_list;

	if (args)
	{
		ft_dprintf(STDERR_FILENO, "env: '%s': No such file or directory\n",
				args->word->word);
		return (127);
	}
	env_list = env(NULL, GET);
	while (env_list)
	{
		if (env_list->key && env_list->value)
			ft_dprintf(STDOUT_FILENO, "%s=%s\n", env_list->key,
					env_list->value);
		env_list = env_list->next;
	}
	return (0);
}

int	builtin_export(t_word_list *args)
{
	t_word_list	*current;
	char		*key;
	char		*value;
	char		*equal_pos;
	t_env		*new_env;

	if (!args)
	{
		show_env_list(env(NULL, GET));
		return (0);
	}
	current = args;
	while (current)
	{
		if (current->word && current->word->word)
		{
			if (!ft_isalpha(current->word->word[0])
				&& current->word->word[0] != '_')
			{
				ft_dprintf(STDERR_FILENO,
							"minishell: export: `%s': not a valid identifier\n",
							current->word->word);
				current = current->next;
				continue ;
			}
			equal_pos = ft_strchr(current->word->word, '=');
			if (equal_pos)
			{
				*equal_pos = '\0';
				key = ft_strdup(current->word->word);
				value = ft_strdup(equal_pos + 1);
				*equal_pos = '=';
				if (key && value)
				{
					new_env = make_env_list(key, value);
					if (new_env)
						add_env_list(env(NULL, GET), new_env);
					else
					{
						free(key);
						free(value);
					}
				}
				else
				{
					if (key)
						free(key);
					if (value)
						free(value);
				}
			}
			else
			{
				key = ft_strdup(current->word->word);
				value = ft_strdup("");
				if (key && value)
				{
					new_env = make_env_list(key, value);
					if (new_env)
						add_env_list(env(NULL, GET), new_env);
					else
					{
						free(key);
						free(value);
					}
				}
				else
				{
					if (key)
						free(key);
					if (value)
						free(value);
				}
			}
		}
		current = current->next;
	}
	return (0);
}

int	builtin_unset(t_word_list *args)
{
	t_word_list *current;
	t_env *env_list;
	t_env *prev;
	t_env *to_remove;

	current = args;
	while (current)
	{
		if (current->word && current->word->word)
		{
			env_list = env(NULL, GET);
			prev = NULL;
			while (env_list)
			{
				if (ft_strcmp(env_list->key, current->word->word) == 0)
				{
					to_remove = env_list;
					if (prev)
						prev->next = env_list->next;
					else
						env(env_list->next, SET);
					free(to_remove->key);
					free(to_remove->value);
					free(to_remove);
					break ;
				}
				prev = env_list;
				env_list = env_list->next;
			}
		}
		current = current->next;
	}
	return (0);
}