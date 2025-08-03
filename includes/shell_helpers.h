/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_helpers.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 03:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_HELPERS_H
# define SHELL_HELPERS_H

// Forward declarations
int			execute_command_parts(char *cmd_str, char delimiter);
const char	*skip_whitespace(const char *str);

// shell_command_execution.c
int			execute_single_command(char *trimmed);
int			process_command_part(char *trimmed, char delimiter);

// shell_input_processing.c
int			execute_command_string(const char *command_str);
char		*read_all_input(void);
int			process_input(char *full_input);

#endif