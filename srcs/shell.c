#include "minishell.h"
#include "signal_handler.h"

int	shell(int argc, char **argv, char **envp)
{
	if (argc > 1)
	{
		ft_dprintf(STDERR_FILENO, "%s: %s: No such file or directory\n", ENAME, argv[1]);
		return (127);
	}
	initialize_enviroment(envp);
	shell_initialize();
	reader_loop();
	exit_shell();
	return (exit_value(0, GET));
}
