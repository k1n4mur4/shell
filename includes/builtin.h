/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:36:22 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 01:36:23 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "command.h"
# include "env.h"
# include "exit_value.h"
# include "ft_printf.h"
# include "libft.h"
# include <errno.h>
# include <limits.h>
# include <string.h>
# include <unistd.h>

int	builtin_echo(t_word_list *args);
int	builtin_cd(t_word_list *args);
int	builtin_pwd(t_word_list *args);
int	builtin_export(t_word_list *args);
int	builtin_unset(t_word_list *args);
int	builtin_env(t_word_list *args);
int	builtin_exit(t_word_list *args);

int	is_builtin_command(const char *command);
int	execute_builtin(const char *command, t_word_list *args);

#endif