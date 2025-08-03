/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:36:29 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:26:37 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_H
# define DEFINE_H

# include <signal.h>

# define ENAME "minishell"
# define PROMPT "minishell$ "
# define ERROR_PREFIX "minishell: "
# define ERROR_SYNTAX_PREFIX "minishell: syntax error: "
# define ERROR_COMMAND_PREFIX "minishell: command not found: "
# define ERROR_PERMISSION_PREFIX "minishell: permission denied: "
# define ERROR_NO_SUCH_FILE "minishell: no such file or directory: "

typedef enum e_type
{
	SET,
	GET,
	SHOW,
	ADD,
	FREE,
	RESET
}	t_type;

#endif