#include "eval.h"
#include "signal_handler.h"

int	reader_loop(void)
{
	t_command			command;

	set_signal();
	while (1)
	{
		/* Reset signal flag before reading input */
		g_signal_received = 0;
		
		command.current_command = readline(ft_strjoin(ENAME, "$ "));
		
		/* Handle Ctrl+D (EOF) */
		if (!command.current_command)
		{
			write(2, "exit\n", 5);
			break ;
		}
		
		/* Handle SIGINT received during readline */
		if (g_signal_received == SIGINT)
		{
			g_signal_received = 0;
			continue ;
		}
		
		/* Skip empty commands */
		if (!*(command.current_command))
		{
			free(command.current_command);
			continue ;
		}
		
		set_deftext(command.current_command);
		parser(&command);
		execute_command(&command);
		dispose_command(&command);
	}
	return (exit_value(0, GET));
}
