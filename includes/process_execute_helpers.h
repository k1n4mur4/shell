/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_execute_helpers.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 04:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_EXECUTE_HELPERS_H
# define PROCESS_EXECUTE_HELPERS_H

# include <sys/types.h>
# include "command.h"

int		wait_for_child_process(pid_t pid);
char	**helper_create_argv(const char *command_path,
			const char *command_name, t_word_list *args);

#endif