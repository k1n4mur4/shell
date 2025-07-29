/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:49:23 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 01:49:24 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exit_shell.h"
#include "redirect_utils.h"

void	exit_shell(void)
{
	rl_clear_history();
	env(NULL, FREE);
}
