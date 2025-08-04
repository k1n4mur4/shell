/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_input_helpers.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 04:30:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:30:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_INPUT_HELPERS_H
# define SHELL_INPUT_HELPERS_H

char	*read_line_with_read(void);
char	*helper_append_input_line(char *full_input, char *line);

#endif