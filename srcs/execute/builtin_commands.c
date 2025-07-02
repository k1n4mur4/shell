#include "builtin.h"
#include "eval.h"

/* Check if command is a builtin */
int	is_builtin_command(const char *command)
{
	if (!command)
		return (0);
	if (ft_strcmp(command, "echo") == 0)
		return (1);
	if (ft_strcmp(command, "cd") == 0)
		return (1);
	if (ft_strcmp(command, "pwd") == 0)
		return (1);
	if (ft_strcmp(command, "export") == 0)
		return (1);
	if (ft_strcmp(command, "unset") == 0)
		return (1);
	if (ft_strcmp(command, "env") == 0)
		return (1);
	if (ft_strcmp(command, "exit") == 0)
		return (1);
	return (0);
}

/* Execute builtin command */
int	execute_builtin(const char *command, t_word_list *args)
{
	if (ft_strcmp(command, "echo") == 0)
		return (builtin_echo(args));
	if (ft_strcmp(command, "cd") == 0)
		return (builtin_cd(args));
	if (ft_strcmp(command, "pwd") == 0)
		return (builtin_pwd(args));
	if (ft_strcmp(command, "export") == 0)
		return (builtin_export(args));
	if (ft_strcmp(command, "unset") == 0)
		return (builtin_unset(args));
	if (ft_strcmp(command, "env") == 0)
		return (builtin_env(args));
	if (ft_strcmp(command, "exit") == 0)
		return (builtin_exit(args));
	return (127);
}

/* Process escape sequences in string for echo -e */
static void	echo_escape_sequences(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1])
		{
			i++;
			if (str[i] == 'n')
				ft_dprintf(STDOUT_FILENO, "\n");
			else if (str[i] == 't')
				ft_dprintf(STDOUT_FILENO, "\t");
			else if (str[i] == 'r')
				ft_dprintf(STDOUT_FILENO, "\r");
			else if (str[i] == 'b')
				ft_dprintf(STDOUT_FILENO, "\b");
			else if (str[i] == 'a')
				ft_dprintf(STDOUT_FILENO, "\a");
			else if (str[i] == 'v')
				ft_dprintf(STDOUT_FILENO, "\v");
			else if (str[i] == 'f')
				ft_dprintf(STDOUT_FILENO, "\f");
			else if (str[i] == '\\')
				ft_dprintf(STDOUT_FILENO, "\\");
			else
			{
				ft_dprintf(STDOUT_FILENO, "\\");
				ft_dprintf(STDOUT_FILENO, "%c", str[i]);
			}
		}
		else
			ft_dprintf(STDOUT_FILENO, "%c", str[i]);
		i++;
	}
}

/* echo builtin command */
int	builtin_echo(t_word_list *args)
{
	int			newline;
	int			interpret_escapes;
	t_word_list	*current;

	newline = 1;
	interpret_escapes = 0;
	current = args;
	
	/* Check for flags */
	while (current && current->word && current->word->word && current->word->word[0] == '-')
	{
		if (ft_strcmp(current->word->word, "-n") == 0)
		{
			newline = 0;
			current = current->next;
		}
		else if (ft_strcmp(current->word->word, "-e") == 0)
		{
			interpret_escapes = 1;
			current = current->next;
		}
		else if (ft_strcmp(current->word->word, "-ne") == 0 || ft_strcmp(current->word->word, "-en") == 0)
		{
			newline = 0;
			interpret_escapes = 1;
			current = current->next;
		}
		else
			break;
	}
	
	/* Print arguments */
	while (current)
	{
		if (current->word && current->word->word)
		{
			if (interpret_escapes)
				echo_escape_sequences(current->word->word);
			else
				ft_dprintf(STDOUT_FILENO, "%s", current->word->word);
			
			if (current->next)
				ft_dprintf(STDOUT_FILENO, " ");
		}
		current = current->next;
	}
	if (newline)
		ft_dprintf(STDOUT_FILENO, "\n");
	return (0);
}

/* pwd builtin command */
int	builtin_pwd(t_word_list *args)
{
	char	*cwd;

	(void)args;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_dprintf(STDERR_FILENO, "minishell: pwd: %s\n", strerror(errno));
		return (1);
	}
	ft_dprintf(STDOUT_FILENO, "%s\n", cwd);
	free(cwd);
	return (0);
}

/* env builtin command */
int	builtin_env(t_word_list *args)
{
	t_env	*env_list;

	/* env command doesn't take arguments in standard implementation */
	if (args)
	{
		ft_dprintf(STDERR_FILENO, "env: '%s': No such file or directory\n", args->word->word);
		return (127);
	}
	
	env_list = env(NULL, GET);
	while (env_list)
	{
		if (env_list->key && env_list->value)
			ft_dprintf(STDOUT_FILENO, "%s=%s\n", env_list->key, env_list->value);
		env_list = env_list->next;
	}
	return (0);
}

/* Check if string is a valid number */
static int	is_valid_number(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	
	if (!str[i])
		return (0);
	
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/* exit builtin command */
int	builtin_exit(t_word_list *args)
{
	int			exit_code;
	long long	temp_code;

	exit_code = 0;
	ft_dprintf(STDOUT_FILENO, "exit\n");
	
	if (args && args->word && args->word->word)
	{
		/* Check for too many arguments */
		if (args->next)
		{
			ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
			return (1);
		}
		
		/* Validate numeric argument */
		if (!is_valid_number(args->word->word))
		{
			ft_dprintf(STDERR_FILENO, "minishell: exit: %s: numeric argument required\n", 
				args->word->word);
			exit_code = 2;
		}
		else
		{
			temp_code = ft_atoi(args->word->word);
			/* Ensure exit code is in valid range (0-255) */
			exit_code = (int)(temp_code & 255);
		}
	}
	
	/* Set exit code and flag shell for termination */
	exit_value(exit_code, SET);
	set_shell_exit_flag(1);
	
	/* Return exit code (shell will terminate via flag check) */
	return (exit_code);
}

/* cd builtin command */
int	builtin_cd(t_word_list *args)
{
	char	*path;
	char	*home;
	t_env	*env_list;

	/* Check for too many arguments */
	if (args && args->next)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: too many arguments\n");
		return (1);
	}

	if (!args || !args->word || !args->word->word)
	{
		env_list = env(NULL, GET);
		home = NULL;
		while (env_list)
		{
			if (ft_strcmp(env_list->key, "HOME") == 0)
			{
				home = env_list->value;
				break ;
			}
			env_list = env_list->next;
		}
		if (!home)
		{
			ft_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
			return (1);
		}
		path = home;
	}
	else
		path = args->word->word;
	if (chdir(path) != 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	return (0);
}

/* export builtin command */
int	builtin_export(t_word_list *args)
{
	t_word_list	*current;
	char		*key;
	char		*value;
	char		*equal_pos;
	t_env		*new_env;

	if (!args)
	{
		show_env_list(env(NULL, GET));
		return (0);
	}
	current = args;
	while (current)
	{
		if (current->word && current->word->word)
		{
			/* Check for valid variable name */
			if (!ft_isalpha(current->word->word[0]) && current->word->word[0] != '_')
			{
				ft_dprintf(STDERR_FILENO, "minishell: export: `%s': not a valid identifier\n", 
					current->word->word);
				current = current->next;
				continue;
			}
			
			equal_pos = ft_strchr(current->word->word, '=');
			if (equal_pos)
			{
				*equal_pos = '\0';
				key = ft_strdup(current->word->word);
				value = ft_strdup(equal_pos + 1);
				*equal_pos = '=';
				if (key && value)
				{
					new_env = make_env_list(key, value);
					if (new_env)
						add_env_list(env(NULL, GET), new_env);
					else
					{
						free(key);
						free(value);
					}
				}
				else
				{
					if (key)
						free(key);
					if (value)
						free(value);
				}
			}
			else
			{
				key = ft_strdup(current->word->word);
				value = ft_strdup("");
				if (key && value)
				{
					new_env = make_env_list(key, value);
					if (new_env)
						add_env_list(env(NULL, GET), new_env);
					else
					{
						free(key);
						free(value);
					}
				}
				else
				{
					if (key)
						free(key);
					if (value)
						free(value);
				}
			}
		}
		current = current->next;
	}
	return (0);
}

/* unset builtin command */
int	builtin_unset(t_word_list *args)
{
	t_word_list	*current;
	t_env		*env_list;
	t_env		*prev;
	t_env		*to_remove;

	current = args;
	while (current)
	{
		if (current->word && current->word->word)
		{
			env_list = env(NULL, GET);
			prev = NULL;
			while (env_list)
			{
				if (ft_strcmp(env_list->key, current->word->word) == 0)
				{
					to_remove = env_list;
					if (prev)
						prev->next = env_list->next;
					else
						env(env_list->next, SET);
					free(to_remove->key);
					free(to_remove->value);
					free(to_remove);
					break ;
				}
				prev = env_list;
				env_list = env_list->next;
			}
		}
		current = current->next;
	}
	return (0);
}