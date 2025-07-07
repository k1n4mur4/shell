#include "minishell.h"
#include "signal_handler.h"
#include "redirect_utils.h"
#include "make_cmd.h"
#include <unistd.h>

/* Forward declaration */
static int	execute_command_parts(char *cmd_str, char delimiter);

/* Skip whitespace characters */
static const char	*skip_whitespace(const char *str)
{
	while (str && *str && (*str == ' ' || *str == '\t' || *str == '\n'))
		str++;
	return (str);
}

/* Execute a single command */
static int	execute_single_command(char *trimmed)
{
	t_command	command;

	command.current_command = ft_strdup(trimmed);
	if (command.current_command)
	{
		parser(&command);
		execute_command(&command);
		dispose_current_command(&command);
		return (exit_value(0, GET));
	}
	return (0);
}

/* Process command part based on delimiter */
static int	process_command_part(char *trimmed, char delimiter)
{
	if (delimiter == '\n')
		return (execute_command_parts(trimmed, ';'));
	else
		return (execute_single_command(trimmed));
}

/* Split command string by delimiter and execute each part */
static int	execute_command_parts(char *cmd_str, char delimiter)
{
	char	*part;
	char	*next_delim;
	char	*trimmed;
	int		last_exit_code;

	last_exit_code = 0;
	part = cmd_str;
	while (part && *part)
	{
		next_delim = ft_strchr(part, delimiter);
		if (next_delim)
			*next_delim = '\0';
		trimmed = (char *)skip_whitespace(part);
		if (*trimmed)
		{
			last_exit_code = process_command_part(trimmed, delimiter);
			if (should_shell_exit())
				break;
		}
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

/* Wrapper function to make get_next_line work like readline */
static char	*my_readline(void)
{
	char	*line;
	size_t	bytes_read;

	line = NULL;
	bytes_read = get_next_line(STDIN_FILENO, &line);
	if (bytes_read > 0)
		return (line);
	if (line)
		free(line);
	return (NULL);
}

/* Process single line from stdin */
static int	process_stdin_line(char *line)
{
	int	result;

	if (line && ft_strlen(line) > 0 && 
		line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	if (line && *line)
	{
		result = execute_command_string(line);
		if (should_shell_exit())
			return (-1);
		return (result);
	}
	return (0);
}

/* Execute commands from stdin (non-interactive mode) */
static int	execute_stdin_commands(void)
{
	char	*line;
	int		last_exit_code;
	int		result;

	last_exit_code = 0;
	while ((line = my_readline()) != NULL)
	{
		result = process_stdin_line(line);
		if (result == -1)
		{
			free(line);
			break;
		}
		if (result != 0)
			last_exit_code = result;
		free(line);
	}
	return (last_exit_code);
}

/* Handle command line option -c */
static int	handle_command_option(char **argv, char **envp)
{
	int	result;

	initialize_enviroment(envp);
	init_temp_file_cleanup();
	shell_initialize();
	result = execute_command_string(argv[2]);
	exit_shell();
	return (result);
}

/* Initialize and run interactive shell */
static void	run_shell_main(char **envp)
{
	initialize_enviroment(envp);
	init_temp_file_cleanup();
	shell_initialize();
	if (isatty(STDIN_FILENO))
		reader_loop();
	else
		execute_stdin_commands();
	exit_shell();
}

int	shell(int argc, char **argv, char **envp)
{
	if (argc >= 3 && ft_strcmp(argv[1], "-c") == 0)
		return (handle_command_option(argv, envp));
	if (argc > 1)
	{
		ft_dprintf(STDERR_FILENO, 
			"%s: %s: No such file or directory\n", ENAME, argv[1]);
		return (127);
	}
	run_shell_main(envp);
	return (exit_value(0, GET));
}
