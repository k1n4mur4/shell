#include "env.h"

t_env	*alloc_env(void)
{
	t_env	*temp;

	temp = (t_env *)ft_calloc(1, sizeof(t_env));
	if (!temp)
		return (NULL);
	temp->key = NULL;
	temp->value = NULL;
	temp->next = NULL;
	return (temp);
}

t_env	*make_env_list(char *key, char *value)
{
	t_env	*temp;

	if (!key)
		return (NULL);
	temp = alloc_env();
	if (!temp)
		return (NULL);
	temp->key = key;
	temp->value = value;
	return (temp);
}

t_env	*initialize_env(char *envp)
{
	int	i;
	t_env	*new;
	char	*key;
	char	*value;

	i = 0;
	while (envp[i] != '=' && envp[i] != '\0')
		i++;
	if (envp[i] == '\0')
		return (NULL);
	key = ft_strndup(envp, i);
	if (!key)
		return (NULL);
	value = ft_strdup(envp + i + 1);
	if (!value)
	{
		free(key);
		return (NULL);
	}
	new = make_env_list(key, value);
	if (!new)
	{
		free(key);
		free(value);
		return (NULL);
	}
	return (new);
}

void	initialize_enviroment(char **envp)
{
	int	count;

	if (!envp || !*envp)
	{
		/* Debug: ft_dprintf(STDERR_FILENO, "Warning: No environment variables found\n"); */
		return;
	}
	count = 0;
	env(initialize_env(*envp), SET);
	envp++;
	count++;
	while (*envp)
	{
		env(initialize_env(*envp), ADD);
		envp++;
		count++;
	}
	/* Debug: ft_dprintf(STDERR_FILENO, "Initialized %d environment variables\n", count); */
}
