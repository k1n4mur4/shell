/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:12 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "process_utils.h"
#include "signal_handler.h"
#include "memory_utils.h"
#include "process_array_helpers.h"

int	count_word_list(t_word_list *list)
{
	int	count;

	count = 0;
	while (list)
	{
		count++;
		list = list->next;
	}
	return (count);
}

char	**create_argv_array(const char *command_path, const char *command_name,
		t_word_list *args)
{
	char	**argv;
	int		argc;

	if (!command_path)
		return (NULL);
	argc = 1 + count_word_list(args);
	argv = allocate_argv(argc, command_name);
	if (!argv)
		return (NULL);
	if (!copy_args(argv, args, argc))
	{
		free_string_array(argv);
		return (NULL);
	}
	argv[argc] = NULL;
	return (argv);
}

char	**create_envp_array(void)
{
	t_env	*env_list;
	int		count;

	env_list = env(NULL, GET);
	if (!env_list)
		return (NULL);
	count = count_env_vars(env_list);
	return (fill_envp_array(env_list, count));
}
