#include "minishell.h"
#include "signal_handler.h"
#include "redirect_utils.h"
#include <unistd.h>

/* Skip whitespace characters */
static const char	*skip_whitespace(const char *str)
{
	while (str && *str && (*str == ' ' || *str == '\t' || *str == '\n'))
		str++;
	return (str);
}

/* Split command string by delimiter and execute each part */
static int	execute_command_parts(char *cmd_str, char delimiter)
{
	char	*part;
	char	*next_delim;
	char	*trimmed;
	t_command	command;
	int		last_exit_code;

	last_exit_code = 0;
	part = cmd_str;
	
	while (part && *part)
	{
		/* Find next delimiter */
		next_delim = ft_strchr(part, delimiter);
		if (next_delim)
			*next_delim = '\0';
		
		/* Skip whitespace */
		trimmed = (char *)skip_whitespace(part);
		
		/* Execute non-empty parts, handling semicolons if delimiter is newline */
		if (*trimmed)
		{
			if (delimiter == '\n')
			{
				/* Further split by semicolons */
				last_exit_code = execute_command_parts(trimmed, ';');
			}
			else
			{
				/* Execute single command */
				command.current_command = ft_strdup(trimmed);
				if (command.current_command)
				{
					parser(&command);
					execute_command(&command);
					dispose_command(&command);
					last_exit_code = exit_value(0, GET);
				}
			}
			
			/* Check for exit flag */
			if (should_shell_exit())
				break;
		}
		
		/* Move to next part */
		if (next_delim)
			part = next_delim + 1;
		else
			break;
	}
	
	return (last_exit_code);
}

/* Execute command from -c option */
static int	execute_command_string(const char *command_str)
{
	const char	*trimmed;
	char		*cmd_copy;
	int			last_exit_code;

	if (!command_str)
		return (0);
	
	/* Skip leading whitespace */
	trimmed = skip_whitespace(command_str);
	
	/* If command is empty or only whitespace, do nothing */
	if (!*trimmed)
		return (0);
	
	/* Create a copy to modify */
	cmd_copy = ft_strdup(command_str);
	if (!cmd_copy)
		return (1);
	
	/* Split by newlines, handling semicolons within each line */
	last_exit_code = execute_command_parts(cmd_copy, '\n');
	
	free(cmd_copy);
	return (last_exit_code);
}

/* Execute commands from stdin (non-interactive mode) */
static int	execute_stdin_commands(void)
{
	char	*line;
	int		last_exit_code;
	size_t	bytes_read;

	last_exit_code = 0;
	line = NULL;
	while ((bytes_read = get_next_line(STDIN_FILENO, &line)) > 0)
	{
		/* Remove trailing newline if present */
		if (line && ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		
		/* Skip empty lines */
		if (line && *line)
		{
			last_exit_code = execute_command_string(line);
			if (should_shell_exit())
			{
				free(line);
				break;
			}
		}
		
		if (line)
		{
			free(line);
			line = NULL;
		}
	}
	
	/* Clean up if line was allocated but not freed */
	if (line)
		free(line);
	
	return (last_exit_code);
}

int	shell(int argc, char **argv, char **envp)
{
	int	i;
	
	/* Handle -c option */
	if (argc >= 3 && ft_strcmp(argv[1], "-c") == 0)
	{
		initialize_enviroment(envp);
		init_temp_file_cleanup();
		shell_initialize();
		
		/* Execute command from -c argument */
		i = execute_command_string(argv[2]);
		
		exit_shell();
		return (i);
	}
	
	/* Handle other arguments (currently not supported) */
	if (argc > 1)
	{
		ft_dprintf(STDERR_FILENO, "%s: %s: No such file or directory\n", ENAME, argv[1]);
		return (127);
	}
	
	/* Initialize shell */
	initialize_enviroment(envp);
	init_temp_file_cleanup();
	shell_initialize();
	
	/* Check if stdin is a terminal (interactive mode) */
	if (isatty(STDIN_FILENO))
	{
		/* Interactive mode: use readline */
		reader_loop();
	}
	else
	{
		/* Non-interactive mode: read from stdin */
		execute_stdin_commands();
	}
	
	exit_shell();
	return (exit_value(0, GET));
}
