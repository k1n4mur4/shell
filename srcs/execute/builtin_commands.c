#include "builtin.h"

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

/* echo builtin command */
int	builtin_echo(t_word_list *args)
{
	int			newline;
	t_word_list	*current;

	newline = 1;
	current = args;
	/* Check for -n flag */
	if (current && current->word && current->word->word &&
		ft_strcmp(current->word->word, "-n") == 0)
	{
		newline = 0;
		current = current->next;
	}
	/* Print arguments */
	while (current)
	{
		if (current->word && current->word->word)
		{
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

	(void)args;
	env_list = env(NULL, GET);
	if (!env_list)
		ft_dprintf(STDERR_FILENO, "Error: No environment variables available\n");
	else
	{
		ft_dprintf(STDERR_FILENO, "DEBUG: Environment list found, displaying...\n");
		show_env_list(env_list);
	}
	return (0);
}

/* exit builtin command */
int	builtin_exit(t_word_list *args)
{
	int	exit_code;

	exit_code = 0;
	ft_dprintf(STDOUT_FILENO, "exit\n");
	if (args && args->word && args->word->word)
	{
		exit_code = ft_atoi(args->word->word);
		if (args->next)
		{
			ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
			return (1);
		}
	}
	exit_value(exit_code, SET);
	exit(exit_code);
	return (exit_code);
}

/* cd builtin command */
int	builtin_cd(t_word_list *args)
{
	char	*path;
	char	*home;
	t_env	*env_list;

	if (!args || !args->word || !args->word->word)
	{
		env_list = env(NULL, GET);
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
			equal_pos = ft_strchr(current->word->word, '=');
			if (equal_pos)
			{
				*equal_pos = '\0';
				key = current->word->word;
				value = equal_pos + 1;
				new_env = make_env_list(key, value);
				if (new_env)
					add_env_list(env(NULL, GET), new_env);
				*equal_pos = '=';
			}
			else
			{
				key = current->word->word;
				new_env = make_env_list(key, "");
				if (new_env)
					add_env_list(env(NULL, GET), new_env);
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