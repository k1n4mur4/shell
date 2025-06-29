#include "minishell.h"

int	exit_value(int exit_value, enum e_type type)
{
	static int	last_command_exit_value = 0;

	if (type == SET)
	{
		last_command_exit_value = exit_value;
	}
	else if (type == GET)
	{
		exit_value = last_command_exit_value;
	}
	return (exit_value);
}

int	shell(int argc, char **argv, char **envp)
{
	if (argc > 1)
	{
		ft_dprintf(STDERR_FILENO, "%s: %s: No such file or directory\n", ENAME, argv[1]);
		return (127);
	}
	initialize_enviroment(envp);
	// shell_initialize();
	// set_bash_input();
	// reader_loop();
	exit_shell();
	return (exit_value(0, GET));
}
