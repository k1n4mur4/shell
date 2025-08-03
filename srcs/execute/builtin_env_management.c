/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_management.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:47:54 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:48:33 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "builtin_env_helpers.h"

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
			if (validate_identifier(current->word->word))
			{
				if (ft_strchr(current->word->word, '='))
					process_with_equal(current->word->word);
				else
					process_without_equal(current->word->word);
			}
		}
		current = current->next;
	}
	return (0);
}

int	builtin_unset(t_word_list *args)
{
	t_word_list	*current;

	current = args;
	while (current)
	{
		if (current->word && current->word->word)
			find_and_remove_env(current->word->word);
		current = current->next;
	}
	return (0);
}
