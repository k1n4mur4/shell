#ifndef	MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include "exit_shell.h"
#include "env.h"
#include "libft.h"
#include "eval.h"
#include "define.h"

// shell.c
int	shell(int argc, char **argv, char **envp);

// main.c
int	main(int argc, char **argv, char **envp);

#endif