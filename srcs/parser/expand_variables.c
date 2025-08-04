/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:35 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:06:57 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "eval.h"
#include "parser.h"

char	*get_env_value(const char *var_name)
{
	t_env	*env_list;

	env_list = env(NULL, GET);
	while (env_list)
	{
		if (ft_strcmp(env_list->key, var_name) == 0)
		{
			if (env_list->value)
				return (ft_strdup(env_list->value));
			else
				return (ft_strdup(""));
		}
		env_list = env_list->next;
	}
	return (ft_strdup(""));
}

char	*get_special_var(char c)
{
	if (c == '?')
		return (ft_itoa(exit_value(0, GET)));
	return (ft_strdup(""));
}

static char	*handle_special_vars(const char *str, int *advance)
{
	char	*var_name;

	*advance = 2;
	var_name = ft_calloc(2, sizeof(char));
	if (var_name)
		var_name[0] = str[1];
	return (var_name);
}

static char	*parse_variable_name(const char *str, int *advance)
{
	int		i;
	char	*var_name;

	i = 1;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (i == 1)
	{
		*advance = 1;
		return (ft_strdup(""));
	}
	*advance = i;
	var_name = ft_calloc(i, sizeof(char));
	if (!var_name)
		return (NULL);
	if (i > 1)
		ft_strlcpy(var_name, &str[1], i);
	return (var_name);
}

char	*extract_var_name(const char *str, int *advance)
{
	if (!str || str[0] != '$')
		return (NULL);
	if (str[1] == '?')
		return (handle_special_vars(str, advance));
	return (parse_variable_name(str, advance));
}
