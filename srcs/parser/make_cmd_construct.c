/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_cmd_construct.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:40 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:37:41 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "make_cmd.h"

t_command	*make_simple_command(t_word_list *words, t_redirect *redirects)
{
	t_command			*cmd;
	t_simple_command	*simple;

	cmd = alloc_command();
	if (!cmd)
		return (NULL);
	simple = alloc_simple_command();
	if (!simple)
	{
		free(cmd);
		return (NULL);
	}
	cmd->type = CM_SIMPLE;
	cmd->simple = simple;
	simple->words = words;
	simple->redirects = redirects;
	return (cmd);
}

t_command	*make_binary_command(t_command_type type, t_command *left,
		t_command *right)
{
	t_command	*cmd;

	cmd = alloc_command();
	if (!cmd)
		return (NULL);
	cmd->type = type;
	cmd->left = left;
	cmd->right = right;
	return (cmd);
}

t_redirect	*make_redirect(t_redirect_type type, const char *filename)
{
	t_redirect	*redirect;

	redirect = alloc_redirect();
	if (!redirect)
		return (NULL);
	redirect->type = type;
	if (filename)
		redirect->filename = ft_strdup(filename);
	return (redirect);
}

void	dispose_ast_command(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->simple)
	{
		dispose_words(cmd->simple->words);
		dispose_redirects(cmd->simple->redirects);
		free(cmd->simple);
	}
	if (cmd->left)
		dispose_ast_command(cmd->left);
	if (cmd->right)
		dispose_ast_command(cmd->right);
	if (cmd->current_command)
		free(cmd->current_command);
	free(cmd);
}