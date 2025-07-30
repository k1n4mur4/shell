/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_filesystem.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:40:46 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static void	update_pwd_vars(char *old_pwd, char *new_pwd)
{
	t_env	*env_list;
	t_env	*pwd_node;
	t_env	*oldpwd_node;

	env_list = env(NULL, GET);
	oldpwd_node = search_env(env_list, "OLDPWD");
	if (oldpwd_node)
	{
		free(oldpwd_node->value);
		oldpwd_node->value = old_pwd ? ft_strdup(old_pwd) : NULL;
	}
	else if (old_pwd)
	{
		oldpwd_node = ft_calloc(1, sizeof(t_env));
		if (oldpwd_node)
		{
			oldpwd_node->key = ft_strdup("OLDPWD");
			oldpwd_node->value = ft_strdup(old_pwd);
			oldpwd_node->next = NULL;
			env(oldpwd_node, ADD);
		}
	}
	pwd_node = search_env(env_list, "PWD");
	if (pwd_node)
	{
		free(pwd_node->value);
		pwd_node->value = new_pwd ? ft_strdup(new_pwd) : NULL;
	}
	else if (new_pwd)
	{
		pwd_node = ft_calloc(1, sizeof(t_env));
		if (pwd_node)
		{
			pwd_node->key = ft_strdup("PWD");
			pwd_node->value = ft_strdup(new_pwd);
			pwd_node->next = NULL;
			env(pwd_node, ADD);
		}
	}
}

int	builtin_pwd(t_word_list *args)
{
	char	*cwd;

	(void)args;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_dprintf(STDERR_FILENO, "minishell: pwd: %s\n", strerror(errno));
		return (1);
	}
	ft_dprintf(STDOUT_FILENO, "%s\n", cwd);
	free(cwd);
	return (0);
}

int	builtin_cd(t_word_list *args)
{
	char *path;
	char *home;
	char *current_pwd;
	char *new_pwd;
	t_env *env_list;
	t_env *oldpwd_node;

	if (args && args->next)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: too many arguments\n");
		return (1);
	}
	current_pwd = getcwd(NULL, 0);
	if (!args || !args->word || !args->word->word)
	{
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
			return (1);
		}
		path = home;
	}
	else if (ft_strcmp(args->word->word, "-") == 0)
	{
		env_list = env(NULL, GET);
		oldpwd_node = search_env(env_list, "OLDPWD");
		if (!oldpwd_node || !oldpwd_node->value)
		{
			ft_dprintf(STDERR_FILENO, "minishell: cd: OLDPWD not set\n");
			free(current_pwd);
			return (1);
		}
		path = oldpwd_node->value;
		ft_dprintf(STDOUT_FILENO, "%s\n", path);
	}
	else
		path = args->word->word;

	if (chdir(path) != 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: %s: %s\n", path,
				strerror(errno));
		free(current_pwd);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		ft_dprintf(STDERR_FILENO,
					"minishell: cd: error retrieving current directory\n");
		free(current_pwd);
		return (1);
	}
	update_pwd_vars(current_pwd, new_pwd);
	free(current_pwd);
	free(new_pwd);
	return (0);
}