#ifndef SIGNAL_HANDLER_H
# define SIGNAL_HANDLER_H

# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "define.h"

/* Signal handling functions */
void	signal_handler(int signum);
void	set_signal(void);
void	shell_initialize(void);
void	setup_child_signals(void);
void	setup_parent_signals(void);

#endif