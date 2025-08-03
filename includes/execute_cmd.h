/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:36:46 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 02:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_CMD_H
# define EXECUTE_CMD_H

# include "command.h"
# include "define.h"
# include "exit_value.h"
# include "ft_printf.h"
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipeline_params
{
	int		*pipe_fd;
	pid_t	*left_pid;
	pid_t	*right_pid;
}	t_pipeline_params;

void	execute_command(t_command *command);
int		execute_simple_command(t_simple_command *simple);
int		execute_pipeline(t_command *left, t_command *right);
int		execute_binary_command(t_command *left, t_command *right,
			t_command_type type);
int		execute_pipeline_process(t_command *left, t_command *right,
			t_pipeline_params *params);
int		execute_external_command_wrapper(char *command_name,
			t_word_list *args);

#endif