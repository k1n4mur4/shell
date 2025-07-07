#include "exit_shell.h"
#include "redirect_utils.h"

void	exit_shell(void)
{
	rl_clear_history();
	env(NULL, FREE);
	cleanup_all_temp_files();
}
