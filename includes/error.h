/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:36:39 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 01:36:41 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include "define.h"
# include "exit_value.h"
# include "ft_printf.h"
# include <stdarg.h>

# define ERROR_SYNTAX "unexpected EOF while looking for matching `%c'\n"
# define ERROR_PARSE "parse error near `%s'\n"

void	parser_error(char *iname, const char *format, ...);

#endif