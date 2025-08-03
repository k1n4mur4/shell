/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:07 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "path_utils.h"
#include "path_validation.h"

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

static char	*_helper_check_absolute_path(const char *command)
{
	if (ft_strchr(command, '/'))
	{
		if (is_executable(command))
			return (ft_strdup(command));
		return (NULL);
	}
	return ((char *)-1);
}

static char	*_helper_build_full_path(const char *dir, const char *command)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, command);
	free(temp);
	return (full_path);
}

static char	*_helper_search_in_path_dirs(char **path_dirs, const char *command)
{
	char	*full_path;
	int		i;

	i = 0;
	while (path_dirs[i])
	{
		full_path = _helper_build_full_path(path_dirs[i], command);
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

char	*find_command_path(const char *command)
{
	char	**path_dirs;
	char	*result;

	if (!command)
		return (NULL);
	result = _helper_check_absolute_path(command);
	if (result != (char *)-1)
		return (result);
	path_dirs = get_path_dirs();
	if (!path_dirs)
		return (NULL);
	return (_helper_search_in_path_dirs(path_dirs, command));
}
