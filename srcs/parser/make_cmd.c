#include "make_cmd.h"

/* Allocate command structure */
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

/* Allocate simple command structure */
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

/* Allocate redirect structure */
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

/* Create simple command */
t_command	*make_simple_command(t_word_list *words, t_redirect *redirects)
{
	t_command		*cmd;
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

/* Create binary command (pipe, and, or) */
t_command	*make_binary_command(t_command_type type, t_command *left, t_command *right)
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

/* Create redirect */
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

/* Dispose command structure */
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

/* Dispose redirects */
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