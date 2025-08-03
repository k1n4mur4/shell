/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_debug.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:47 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:51:59 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "parser.h"
#include "parser_debug_helpers.h"

static void	_helper_print_words(t_word_list *words)
{
	while (words)
	{
		ft_dprintf(STDOUT_FILENO, " %s", words->word->word);
		words = words->next;
	}
}

static void	_helper_print_redirect(t_redirect *redir)
{
	if (redir->type == R_INPUT)
		ft_dprintf(STDOUT_FILENO, " <%s", redir->filename);
	else if (redir->type == R_OUTPUT)
		ft_dprintf(STDOUT_FILENO, " >%s", redir->filename);
	else if (redir->type == R_APPEND)
		ft_dprintf(STDOUT_FILENO, " >>%s", redir->filename);
	else if (redir->type == R_HEREDOC)
		ft_dprintf(STDOUT_FILENO, " <<%s", redir->filename);
}

static void	_helper_print_redirects(t_redirect *redirects)
{
	t_redirect	*redir;

	ft_dprintf(STDOUT_FILENO, " [redirects:");
	redir = redirects;
	while (redir)
	{
		_helper_print_redirect(redir);
		redir = redir->next;
	}
	ft_dprintf(STDOUT_FILENO, "]");
}

static void	_helper_print_simple_cmd(t_simple_command *simple)
{
	ft_dprintf(STDOUT_FILENO, "SIMPLE_CMD:");
	if (simple && simple->words)
		_helper_print_words(simple->words);
	else
		ft_dprintf(STDOUT_FILENO, " (empty)");
	if (simple && simple->redirects)
		_helper_print_redirects(simple->redirects);
	ft_dprintf(STDOUT_FILENO, "\n");
}

void	print_ast(t_command *cmd, int depth)
{
	if (!cmd)
		return ;
	helper_print_indent(depth);
	if (cmd->type == CM_SIMPLE)
		_helper_print_simple_cmd(cmd->simple);
	else if (cmd->type == CM_PIPE)
	{
		ft_dprintf(STDOUT_FILENO, "PIPE:\n");
		print_ast(cmd->left, depth + 1);
		print_ast(cmd->right, depth + 1);
	}
	else if (cmd->type == CM_AND)
	{
		ft_dprintf(STDOUT_FILENO, "AND:\n");
		print_ast(cmd->left, depth + 1);
		print_ast(cmd->right, depth + 1);
	}
	else if (cmd->type == CM_OR)
	{
		ft_dprintf(STDOUT_FILENO, "OR:\n");
		print_ast(cmd->left, depth + 1);
		print_ast(cmd->right, depth + 1);
	}
}
