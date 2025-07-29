/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:36:59 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 01:37:01 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "define.h"
# include "env.h"
# include "eval.h"
# include "exit_shell.h"
# include "libft.h"
# include <stdlib.h>

// shell.c
int	shell(int argc, char **argv, char **envp);

// main.c
int	main(int argc, char **argv, char **envp);

#endif