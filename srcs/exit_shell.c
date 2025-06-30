#include "exit_shell.h"

void	exit_shell(void)
{
	rl_clear_history();
	env(NULL, FREE);
}
