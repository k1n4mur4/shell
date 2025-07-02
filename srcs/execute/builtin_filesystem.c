#include "builtin.h"

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

int	builtin_cd(t_word_list *args)
{
	char	*path;
	char	*home;
	t_env	*env_list;

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