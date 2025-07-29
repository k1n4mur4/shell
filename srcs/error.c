/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:58 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 01:48:58 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"

void	parser_error(char *iname, const char *format, ...)
{
	va_list	args;

	if (!iname)
		ft_dprintf(STDERR_FILENO, "%s: ", ENAME);
	else
		ft_dprintf(STDERR_FILENO, "%s: %s: ", ENAME, iname);
	va_start(args, format);
	ft_vdprintf(STDERR_FILENO, format, args);
	va_end(args);
	exit_value(2, SET);
}
