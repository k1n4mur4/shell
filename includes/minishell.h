#ifndef	MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
// #include <readline/readline.h>
// #include <readline/history.h>
#include "type.h"
#include "env.h"
#include "libft.h"

#define ENAME	"minishell"


// exit_shell.c
void	exit_shell(void);

// shell.c
int	shell(int argc, char **argv, char **envp);

// main.c
int	main(int argc, char **argv, char **envp);

#endif