/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_cmd_alloc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:40 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 20:51:15 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "make_cmd.h"

t_command	*alloc_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)ft_calloc(1, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->type = CM_SIMPLE;
	cmd->simple = NULL;
	cmd->left = NULL;
	cmd->right = NULL;
	cmd->current_command = NULL;
	return (cmd);
}

t_simple_command	*alloc_simple_command(void)
{
	t_simple_command	*simple;

	simple = (t_simple_command *)ft_calloc(1, sizeof(t_simple_command));
	if (!simple)
		return (NULL);
	simple->words = NULL;
	simple->redirects = NULL;
	return (simple);
}

t_redirect	*alloc_redirect(void)
{
	t_redirect	*redirect;

	redirect = (t_redirect *)ft_calloc(1, sizeof(t_redirect));
	if (!redirect)
		return (NULL);
	redirect->type = R_INPUT;
	redirect->filename = NULL;
	redirect->next = NULL;
	return (redirect);
}

void	dispose_redirects(t_redirect *redirects)
{
	t_redirect	*next;

	while (redirects)
	{
		next = redirects->next;
		if (redirects->filename)
			free(redirects->filename);
		free(redirects);
		redirects = next;
	}
}
