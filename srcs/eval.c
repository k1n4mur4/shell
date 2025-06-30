#include "eval.h"
#include "signal_handler.h"

/*
 * Main REPL (Read-Eval-Print Loop) for the minishell
 * 
 * This function handles:
 * - Signal management (SIGINT/SIGQUIT)
 * - User input reading with readline
 * - Command parsing and execution
 * - Memory management for input and command structures
 * - Proper cleanup and error handling
 * 
 * Returns: exit status of the last executed command
 */
int	reader_loop(void)
{
	t_command			command;
	char				*prompt;

	/* Initialize signal handlers for interactive mode */
	set_signal();
	
	while (1)
	{
		/* Reset signal flag before reading input to handle Ctrl+C */
		g_signal_received = 0;
		
		/* Create prompt string and read user input */
		prompt = ft_strjoin(ENAME, "$ ");
		if (!prompt)
			break ;  /* Memory allocation failure - exit gracefully */
		
		command.current_command = readline(prompt);
		free(prompt);  /* Free prompt immediately after use */
		
		/* Handle Ctrl+D (EOF) - user wants to exit */
		if (!command.current_command)
		{
			write(2, "exit\n", 5);
			break ;
		}
		
		/* Handle SIGINT received during readline - start new prompt */
		if (g_signal_received == SIGINT)
		{
			g_signal_received = 0;
			free(command.current_command);
			continue ;
		}
		
		/* Skip empty commands (just whitespace or empty input) */
		if (!*(command.current_command))
		{
			free(command.current_command);
			continue ;
		}
		
		/* Process valid command: parse, execute, cleanup */
		set_deftext(command.current_command);  /* Add to history */
		parser(&command);                      /* Parse into AST */
		execute_command(&command);             /* Execute parsed command */
		dispose_command(&command);             /* Clean up memory */
	}
	
	return (exit_value(0, GET));
}
