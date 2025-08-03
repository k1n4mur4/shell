/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:47:51 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:50:16 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "builtin_echo_helpers.h"

static void	echo_escape_sequences(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1])
		{
			i++;
			print_escape_char(str[i]);
		}
		else
			ft_fputc(str[i], STDOUT_FILENO);
		i++;
	}
}

static t_word_list	*check_option_format(t_word_list *current, int *newline,
		int *interpret_escapes)
{
	if (ft_strcmp(current->word->word, "-n") == 0)
	{
		*newline = 0;
		return (current->next);
	}
	else if (ft_strcmp(current->word->word, "-e") == 0)
	{
		*interpret_escapes = 1;
		return (current->next);
	}
	else if (ft_strcmp(current->word->word, "-ne") == 0
		|| ft_strcmp(current->word->word, "-en") == 0)
	{
		*newline = 0;
		*interpret_escapes = 1;
		return (current->next);
	}
	return (NULL);
}

static t_word_list	*parse_echo_options(t_word_list *args, int *newline,
		int *interpret_escapes)
{
	t_word_list	*current;
	t_word_list	*next;

	*newline = 1;
	*interpret_escapes = 0;
	current = args;
	while (current && current->word && current->word->word
		&& current->word->word[0] == '-')
	{
		next = check_option_format(current, newline, interpret_escapes);
		if (!next)
			break ;
		current = next;
	}
	return (current);
}

static void	print_echo_args(t_word_list *current, int interpret_escapes)
{
	while (current)
	{
		if (current->word && current->word->word)
		{
			if (interpret_escapes)
				echo_escape_sequences(current->word->word);
			else
				ft_putstr_fd(current->word->word, STDOUT_FILENO);
			if (current->next)
				ft_fputc(' ', STDOUT_FILENO);
		}
		current = current->next;
	}
}

int	builtin_echo(t_word_list *args)
{
	int			newline;
	int			interpret_escapes;
	t_word_list	*current;

	current = parse_echo_options(args, &newline, &interpret_escapes);
	print_echo_args(current, interpret_escapes);
	if (newline)
		ft_fputc('\n', STDOUT_FILENO);
	return (0);
}
