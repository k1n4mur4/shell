#include "builtin.h"
#include "eval.h"

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

int	builtin_exit(t_word_list *args)
{
	int			exit_code;
	long long	temp_code;

	exit_code = 0;
	ft_dprintf(STDOUT_FILENO, "exit\n");
	
	if (args && args->word && args->word->word)
	{
		if (args->next)
		{
			ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
			return (1);
		}
		
		if (!is_valid_number(args->word->word))
		{
			ft_dprintf(STDERR_FILENO, "minishell: exit: %s: numeric argument required\n", 
				args->word->word);
			exit_code = 2;
		}
		else
		{
			temp_code = ft_atoi(args->word->word);
			exit_code = (int)(temp_code & 255);
		}
	}
	
	exit_value(exit_code, SET);
	set_shell_exit_flag(1);
	
	return (exit_code);
}