/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:29 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 21:05:02 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "eval.h"
#include "make_cmd.h"
#include "parser.h"

static char	*process_word_expansion(t_word_desc *word)
{
	char	*expanded;

	if (word->flags & W_SINGLEQUOTED)
		expanded = ft_strdup(word->word);
	else if (word->flags & W_QUOTED)
		expanded = expand_word_with_flags(word->word, word->flags);
	else if (should_expand_in_context(word->word))
		expanded = expand_word(word->word);
	else
		expanded = NULL;
	return (expanded);
}

static void	expand_word_list(t_word_list *words)
{
	t_word_list	*current;
	char		*expanded;

	current = words;
	while (current)
	{
		if (current->word && current->word->word)
		{
			expanded = process_word_expansion(current->word);
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
