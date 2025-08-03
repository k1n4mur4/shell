/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_array_helpers.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 03:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_ARRAY_HELPERS_H
# define PROCESS_ARRAY_HELPERS_H

# include "command.h"
# include "env.h"

// process_array_utils.c
char	**allocate_argv(int argc, const char *command_name);
int		copy_args(char **argv, t_word_list *args, int argc);
int		count_env_vars(t_env *env_list);
char	*create_env_string(t_env *env_node);
char	**fill_envp_array(t_env *env_list, int count);

#endif