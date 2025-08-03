/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_array_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 03:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:48:59 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "process_utils.h"
#include "memory_utils.h"

char	**allocate_argv(int argc, const char *command_name)
{
	char	**argv;

	argv = ft_calloc(argc + 1, sizeof(char *));
	if (!argv)
		return (NULL);
	argv[0] = ft_strdup(command_name);
	if (!argv[0])
	{
		free(argv);
		return (NULL);
	}
	return (argv);
}

int	copy_args(char **argv, t_word_list *args, int argc)
{
	int			i;
	t_word_list	*current;

	i = 1;
	current = args;
	while (current && i < argc)
	{
		if (current->word && current->word->word)
		{
			argv[i] = ft_strdup(current->word->word);
			if (!argv[i])
				return (0);
		}
		current = current->next;
		i++;
	}
	return (1);
}

int	count_env_vars(t_env *env_list)
{
	int		count;
	t_env	*temp_env;

	count = 0;
	temp_env = env_list;
	while (temp_env)
	{
		if (temp_env->key)
			count++;
		temp_env = temp_env->next;
	}
	return (count);
}

char	*create_env_string(t_env *env_node)
{
	char	*temp;
	char	*env_str;

	if (env_node->value)
	{
		temp = ft_strjoin(env_node->key, "=");
		if (!temp)
			return (NULL);
		env_str = ft_strjoin(temp, env_node->value);
		free(temp);
		return (env_str);
	}
	else
		return (ft_strjoin(env_node->key, "="));
}

char	**fill_envp_array(t_env *env_list, int count)
{
	char	**envp;
	char	*env_str;
	int		i;

	envp = ft_calloc(count + 1, sizeof(char *));
	if (!envp)
		return (NULL);
	i = 0;
	while (env_list && i < count)
	{
		if (env_list->key)
		{
			env_str = create_env_string(env_list);
			if (!env_str)
			{
				free_string_array(envp);
				return (NULL);
			}
			envp[i] = env_str;
			i++;
		}
		env_list = env_list->next;
	}
	envp[i] = NULL;
	return (envp);
}
