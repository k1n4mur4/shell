/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:12 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:45:39 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "process_utils.h"
#include "signal_handler.h"

int	count_word_list(t_word_list *list)
{
	int	count;

	count = 0;
	while (list)
	{
		count++;
		list = list->next;
	}
	return (count);
}

char	**create_argv_array(const char *command_path, const char *command_name,
		t_word_list *args)
{
	char		**argv;
	int			argc;
	int			i;
	t_word_list	*current;

	if (!command_path)
		return (NULL);
	argc = 1 + count_word_list(args);
	argv = ft_calloc(argc + 1, sizeof(char *));
	if (!argv)
		return (NULL);
	argv[0] = ft_strdup(command_name);
	if (!argv[0])
	{
		free(argv);
		return (NULL);
	}
	i = 1;
	current = args;
	while (current && i < argc)
	{
		if (current->word && current->word->word)
		{
			argv[i] = ft_strdup(current->word->word);
			if (!argv[i])
			{
				free_argv_array(argv);
				return (NULL);
			}
		}
		current = current->next;
		i++;
	}
	argv[argc] = NULL;
	return (argv);
}

char	**create_envp_array(void)
{
	t_env	*env_list;
	char	**envp;
	char	*env_str;
	char	*temp;
	int		count;
	int		i;
	t_env	*temp_env;

	env_list = env(NULL, GET);
	if (!env_list)
		return (NULL);
	count = 0;
	temp_env = env_list;
	while (temp_env)
	{
		if (temp_env->key)
			count++;
		temp_env = temp_env->next;
	}
	envp = ft_calloc(count + 1, sizeof(char *));
	if (!envp)
		return (NULL);
	i = 0;
	while (env_list && i < count)
	{
		if (env_list->key)
		{
			if (env_list->value)
			{
				temp = ft_strjoin(env_list->key, "=");
				if (!temp)
				{
					free_envp_array(envp);
					return (NULL);
				}
				env_str = ft_strjoin(temp, env_list->value);
				free(temp);
			}
			else
			{
				env_str = ft_strjoin(env_list->key, "=");
			}
			if (!env_str)
			{
				free_envp_array(envp);
				return (NULL);
			}
			envp[i] = env_str;
			i++;
		}
		env_list = env_list->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	free_argv_array(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

/* Free envp array */
void	free_envp_array(char **envp)
{
	int	i;

	if (!envp)
		return ;

	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

int	wait_for_child_process(pid_t pid)
{
	int	status;
	int	exit_code;

	if (waitpid(pid, &status, 0) == -1)
	{
		ft_dprintf(STDERR_FILENO, ERROR_PREFIX "waitpid failed: %s\n",
				strerror(errno));
		return (1);
	}
	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		return (exit_code);
	}
	else if (WIFSIGNALED(status))
	{
		exit_code = 128 + WTERMSIG(status);
		return (exit_code);
	}
	return (1);
}

int	execute_external_command(const char *command_path, const char *command_name,
		t_word_list *args)
{
	pid_t pid;
	char **argv;
	char **envp;
	int exit_code;

	if (!command_path)
		return (127);
	argv = create_argv_array(command_path, command_name, args);
	if (!argv)
		return (1);
	envp = create_envp_array();
	if (!envp)
	{
		free_argv_array(argv);
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		ft_dprintf(STDERR_FILENO, ERROR_PREFIX "fork failed: %s\n",
				strerror(errno));
		free_argv_array(argv);
		free_envp_array(envp);
		return (1);
	}
	else if (pid == 0)
	{
		setup_child_signals();
		if (execve(command_path, argv, envp) == -1)
		{
			ft_dprintf(STDERR_FILENO, ERROR_PREFIX "%s: %s\n", command_name,
					strerror(errno));
			free_argv_array(argv);
			free_envp_array(envp);
			if (errno == EACCES)
				exit(126);
			else
				exit(127);
		}
	}
	else
	{
		setup_parent_signals();
		exit_code = wait_for_child_process(pid);
		set_signal();
		free_argv_array(argv);
		free_envp_array(envp);
		return (exit_code);
	}
	return (1);
}
