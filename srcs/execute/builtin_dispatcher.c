#include "builtin.h"
#include "eval.h"

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