/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_debug.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:47 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 20:50:13 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "parser.h"

void	print_ast(t_command *cmd, int depth)
{
	int			i;
	t_redirect	*redir;
	t_word_list	*words;

	if (!cmd)
		return ;
	i = 0;
	while (i < depth)
	{
		ft_dprintf(STDOUT_FILENO, "  ");
		i++;
	}
	if (cmd->type == CM_SIMPLE)
	{
		ft_dprintf(STDOUT_FILENO, "SIMPLE_CMD:");
		if (cmd->simple && cmd->simple->words)
		{
			words = cmd->simple->words;
			while (words)
			{
				ft_dprintf(STDOUT_FILENO, " %s", words->word->word);
				words = words->next;
			}
		}
		else
			ft_dprintf(STDOUT_FILENO, " (empty)");
		if (cmd->simple && cmd->simple->redirects)
		{
			ft_dprintf(STDOUT_FILENO, " [redirects:");
			redir = cmd->simple->redirects;
			while (redir)
			{
				if (redir->type == R_INPUT)
					ft_dprintf(STDOUT_FILENO, " <%s", redir->filename);
				else if (redir->type == R_OUTPUT)
					ft_dprintf(STDOUT_FILENO, " >%s", redir->filename);
				else if (redir->type == R_APPEND)
					ft_dprintf(STDOUT_FILENO, " >>%s", redir->filename);
				else if (redir->type == R_HEREDOC)
					ft_dprintf(STDOUT_FILENO, " <<%s", redir->filename);
				redir = redir->next;
			}
			ft_dprintf(STDOUT_FILENO, "]");
		}
		ft_dprintf(STDOUT_FILENO, "\n");
	}
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
