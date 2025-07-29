/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:07 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 01:48:08 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "path_utils.h"

/* Check if file is executable */
int	is_executable(const char *path)
{
	struct stat	st;

	if (!path)
		return (0);
	if (stat(path, &st) != 0)
		return (0);
	if (!S_ISREG(st.st_mode))
		return (0);
	if (access(path, X_OK) != 0)
		return (0);
	return (1);
}

/* Get PATH directories as array */
char	**get_path_dirs(void)
{
	t_env	*env_list;
	char	*path_value;

	env_list = env(NULL, GET);
	while (env_list)
	{
		if (ft_strcmp(env_list->key, "PATH") == 0)
		{
			path_value = env_list->value;
			if (path_value)
				return (ft_split(path_value, ':'));
			break ;
		}
		env_list = env_list->next;
	}
	return (NULL);
}

/* Find command in PATH */
char	*find_command_path(const char *command)
{
	char **path_dirs;
	char *full_path;
	char *temp;
	int i;

	if (!command)
		return (NULL);

	/* If command contains '/', treat as absolute/relative path */
	if (ft_strchr(command, '/'))
	{
		if (is_executable(command))
			return (ft_strdup(command));
		return (NULL);
	}

	/* Search in PATH directories */
	path_dirs = get_path_dirs();
	if (!path_dirs)
		return (NULL);

	i = 0;
	while (path_dirs[i])
	{
		temp = ft_strjoin(path_dirs[i], "/");
		if (!temp)
		{
			ft_free_array2((void **)path_dirs);
			return (NULL);
		}
		full_path = ft_strjoin(temp, command);
		free(temp);
		if (!full_path)
		{
			ft_free_array2((void **)path_dirs);
			return (NULL);
		}
		if (is_executable(full_path))
		{
			ft_free_array2((void **)path_dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_array2((void **)path_dirs);
	return (NULL);
}