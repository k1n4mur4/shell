#ifndef	EVAL_H
#define	EVAL_H

#include <stdio.h>
#include <readline/readline.h>

#include "define.h"
#include "command.h"
#include "rl.h"
#include "exit_value.h"
#include "parser.h"
#include "execute_cmd.h"
#include "dispose_cmd.h"
#include "libft.h"

/* Shell termination control */
void	set_shell_exit_flag(int flag);
int		should_shell_exit(void);

int		reader_loop(void);

#endif