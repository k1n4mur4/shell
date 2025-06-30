#include "eval.h"

int	reader_loop(void)
{
	t_command			command;

	// set_signal();
	while (1)
	{
		command.current_command = readline(ft_strjoin(ENAME, "$ "));
		if (!command.current_command)
		{
			write(2, "exit\n", 5);
			break ;
		}
		if (!*(command.current_command))
			continue ;
		set_deftext(command.current_command);
		parser(&command);
		execute_command(&command);
		dispose_command(&command);
	}
	return (exit_value(0, GET));
}
