/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:47 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:39:24 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "parser.h"

void	print_word_list(t_word_list *list)
{
	t_word_list	*current;
	int			index;

	index = 0;
	current = list;
	while (current)
	{
		ft_dprintf(STDOUT_FILENO, "Word[%d]: '%s' (flags: 0x%x)\n", index++,
				current->word->word, current->word->flags);
		current = current->next;
	}
}

int	is_redirect_operator(const char *token)
{
	if (!token)
		return (0);
	if (ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0 ||
		ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0)
		return (1);
	return (0);
}

void	append_redirect(t_redirect **head, t_redirect *new_redirect)
{
	t_redirect	*current;

	if (!head || !new_redirect)
		return ;
	if (!*head)
	{
		*head = new_redirect;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_redirect;
}

t_word_list	*parse_command_line(const char *input)
{
	if (!input || !*input)
		return (NULL);
	return (lexer_tokenize(input));
}

t_command	*parse_tokens_to_ast(t_word_list *tokens)
{
	t_word_list	*token_copy;

	if (!tokens)
		return (NULL);
	token_copy = tokens;
	return (parse_and_or(&token_copy));
}