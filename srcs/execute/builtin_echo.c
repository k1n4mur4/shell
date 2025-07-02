#include "builtin.h"

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

int	builtin_echo(t_word_list *args)
{
	int			newline;
	int			interpret_escapes;
	t_word_list	*current;

	newline = 1;
	interpret_escapes = 0;
	current = args;
	
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