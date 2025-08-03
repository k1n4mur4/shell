/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 03:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:48:17 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	free_key_value(char *key, char *value)
{
	if (key)
		free(key);
	if (value)
		free(value);
}

void	add_env_var(char *key, char *value)
{
	t_env	*new_env;

	if (key && value)
	{
		new_env = make_env_list(key, value);
		if (new_env)
			add_env_list(env(NULL, GET), new_env);
		else
			free_key_value(key, value);
	}
	else
		free_key_value(key, value);
}

void	process_with_equal(char *word)
{
	char	*equal_pos;
	char	*key;
	char	*value;

	equal_pos = ft_strchr(word, '=');
	*equal_pos = '\0';
	key = ft_strdup(word);
	value = ft_strdup(equal_pos + 1);
	*equal_pos = '=';
	add_env_var(key, value);
}

void	process_without_equal(char *word)
{
	char	*key;
	char	*value;

	key = ft_strdup(word);
	value = ft_strdup("");
	add_env_var(key, value);
}

int	validate_identifier(char *word)
{
	if (!ft_isalpha(word[0]) && word[0] != '_')
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: export: `%s': not a valid identifier\n",
			word);
		return (0);
	}
	return (1);
}
