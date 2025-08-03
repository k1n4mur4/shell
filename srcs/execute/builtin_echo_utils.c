/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 03:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:48:47 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	print_escape_char(char c)
{
	if (c == 'n')
		ft_fputc('\n', STDOUT_FILENO);
	else if (c == 't')
		ft_fputc('\t', STDOUT_FILENO);
	else if (c == 'r')
		ft_fputc('\r', STDOUT_FILENO);
	else if (c == 'b')
		ft_fputc('\b', STDOUT_FILENO);
	else if (c == 'a')
		ft_fputc('\a', STDOUT_FILENO);
	else if (c == 'v')
		ft_fputc('\v', STDOUT_FILENO);
	else if (c == 'f')
		ft_fputc('\f', STDOUT_FILENO);
	else if (c == '\\')
		ft_fputc('\\', STDOUT_FILENO);
	else
	{
		ft_fputc('\\', STDOUT_FILENO);
		ft_fputc(c, STDOUT_FILENO);
		return (2);
	}
	return (1);
}
