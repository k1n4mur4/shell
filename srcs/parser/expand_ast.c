/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:29 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:35:52 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "eval.h"
#include "parser.h"

char	*expand_word(const char *word)
{
	int	has_single;
	int	has_double;

	if (!word)
		return (NULL);
	if (ft_strchr(word, '\'') != NULL)
		has_single = 1;
	else
		has_single = 0;
	if (ft_strchr(word, '"') != NULL)
		has_double = 1;
	else
		has_double = 0;
	if (has_single && has_double)
		return (expand_mixed_quotes(word));
	else if (has_single)
		return (expand_single_quoted(word));
	else if (has_double)
		return (expand_double_quoted(word));
	else
		return (expand_unquoted(word));
}

int	should_expand_in_context(const char *word)
{
	if (!word)
		return (0);
	if (ft_strchr(word, '\'') != NULL)
		return (1);
	if (ft_strchr(word, '"') != NULL)
		return (1);
	if (ft_strchr(word, '$') != NULL)
		return (1);
	return (0);
}

static void	expand_word_list(t_word_list *words)
{
	t_word_list	*current;
	char		*expanded;

	current = words;
	while (current)
	{
		if (current->word && current->word->word &&
			should_expand_in_context(current->word->word))
		{
			expanded = expand_word(current->word->word);
			if (expanded)
			{
				free(current->word->word);
				current->word->word = expanded;
				current->word->flags &= ~(W_HASDOLLAR | W_QUOTED);
			}
		}
		current = current->next;
	}
}

static void	expand_redirections(t_redirect *redirects)
{
	t_redirect	*current;
	char		*expanded;

	current = redirects;
	while (current)
	{
		if (current->filename && should_expand_in_context(current->filename))
		{
			expanded = expand_word(current->filename);
			if (expanded)
			{
				free(current->filename);
				current->filename = expanded;
			}
		}
		current = current->next;
	}
}

void	expand_ast(t_command *ast)
{
	if (!ast)
		return ;

	if (ast->type == CM_SIMPLE && ast->simple)
	{
		if (ast->simple->words)
			expand_word_list(ast->simple->words);
		if (ast->simple->redirects)
			expand_redirections(ast->simple->redirects);
	}
	else if (ast->type == CM_PIPE)
	{
		expand_ast(ast->left);
		expand_ast(ast->right);
	}
	else if (ast->type == CM_AND || ast->type == CM_OR)
		expand_ast(ast->left);
}