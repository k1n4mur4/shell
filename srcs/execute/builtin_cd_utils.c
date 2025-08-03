/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 03:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:49:23 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

char	*get_home_path(char *current_pwd)
{
	t_env	*env_list;
	char	*home;

	env_list = env(NULL, GET);
	home = NULL;
	while (env_list)
	{
		if (ft_strcmp(env_list->key, "HOME") == 0)
		{
			home = env_list->value;
			break ;
		}
		env_list = env_list->next;
	}
	if (!home)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
		free(current_pwd);
		return (NULL);
	}
	return (home);
}

char	*get_oldpwd_path(char *current_pwd)
{
	t_env	*env_list;
	t_env	*oldpwd_node;
	char	*path;

	env_list = env(NULL, GET);
	oldpwd_node = search_env(env_list, "OLDPWD");
	if (!oldpwd_node || !oldpwd_node->value)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: OLDPWD not set\n");
		free(current_pwd);
		return (NULL);
	}
	path = oldpwd_node->value;
	ft_dprintf(STDOUT_FILENO, "%s\n", path);
	return (path);
}

char	*determine_target_path(t_word_list *args, char *current_pwd)
{
	if (!args || !args->word || !args->word->word)
		return (get_home_path(current_pwd));
	else if (ft_strcmp(args->word->word, "-") == 0)
		return (get_oldpwd_path(current_pwd));
	else
		return (args->word->word);
}

int	validate_cd_args(t_word_list *args)
{
	if (args && args->next)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: too many arguments\n");
		return (0);
	}
	return (1);
}

int	change_directory(const char *path, char *current_pwd)
{
	if (chdir(path) != 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: %s: %s\n", path,
			strerror(errno));
		free(current_pwd);
		return (0);
	}
	return (1);
}
