/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:36:43 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 01:36:45 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVAL_H
# define EVAL_H

# include "command.h"
# include "define.h"
# include "dispose_cmd.h"
# include "execute_cmd.h"
# include "exit_value.h"
# include "libft.h"
# include "parser.h"
# include "rl.h"
# include <readline/readline.h>
# include <stdio.h>

int	reader_loop(void);

#endif