/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:47 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:39:24 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "parser.h"

static t_command	*parse_pipeline(t_word_list **tokens);
static t_command	*parse_and_or(t_word_list **tokens);
static t_command	*parse_simple_command(t_word_list **tokens);
static int			is_redirect_operator(const char *token);
static void			append_redirect(t_redirect **head, t_redirect *new_redirect);

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

static void	print_ast(t_command *cmd, int depth)
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

static t_command	*parse_and_or(t_word_list **tokens)
{
	t_command	*left;
	t_command	*right;
	int			is_and;

	left = parse_pipeline(tokens);
	if (!left)
		return (NULL);
	while (*tokens && ((*tokens)->word->flags & (W_AND | W_OR)))
	{
		is_and = ((*tokens)->word->flags & W_AND);
		*tokens = (*tokens)->next;
		if (!*tokens)
		{
			parser_error(NULL, ERROR_PARSE, "\\n");
			dispose_ast_command(left);
			return (NULL);
		}
		right = parse_pipeline(tokens);
		if (!right)
		{
			dispose_ast_command(left);
			return (NULL);
		}
		if (is_and)
			left = make_binary_command(CM_AND, left, right);
		else
			left = make_binary_command(CM_OR, left, right);
		if (!left)
		{
			dispose_ast_command(right);
			return (NULL);
		}
	}
	return (left);
}

static t_command	*parse_pipeline(t_word_list **tokens)
{
	t_command	*left;
	t_command	*right;

	left = parse_simple_command(tokens);
	if (!left)
		return (NULL);
	while (*tokens && ((*tokens)->word->flags & W_PIPE))
	{
		*tokens = (*tokens)->next;
		if (!*tokens)
		{
			parser_error(NULL, ERROR_PARSE, "\\n");
			dispose_ast_command(left);
			return (NULL);
		}
		right = parse_simple_command(tokens);
		if (!right)
		{
			dispose_ast_command(left);
			return (NULL);
		}
		left = make_binary_command(CM_PIPE, left, right);
		if (!left)
		{
			dispose_ast_command(right);
			return (NULL);
		}
	}
	return (left);
}

static t_command	*parse_simple_command(t_word_list **tokens)
{
	t_word_list		*words;
	t_redirect		*redirects;
	t_word_list		*current;
	t_redirect_type	type;
	t_redirect		*new_redirect;
	t_word_desc		*word_copy;

	if (!tokens || !*tokens)
		return (NULL);
	words = NULL;
	redirects = NULL;
	while (*tokens)
	{
		current = *tokens;
		if (current->word->flags & W_PIPE)
			break ;
		if (current->word->flags & (W_AND | W_OR))
			break ;
		if (is_redirect_operator(current->word->word))
		{
			if (ft_strcmp(current->word->word, ">>") == 0)
				type = R_APPEND;
			else if (ft_strcmp(current->word->word, "<<") == 0)
				type = R_HEREDOC;
			else if (ft_strcmp(current->word->word, "<") == 0)
				type = R_INPUT;
			else if (ft_strcmp(current->word->word, ">") == 0)
				type = R_OUTPUT;
			else
			{
				*tokens = (*tokens)->next;
				continue ;
			}
			*tokens = (*tokens)->next;
			if (!*tokens || !(*tokens)->word)
			{
				parser_error(NULL, ERROR_PARSE, "\\n");
				dispose_words(words);
				dispose_redirects(redirects);
				return (NULL);
			}
			if (is_redirect_operator((*tokens)->word->word) ||
				((*tokens)->word->flags & W_PIPE) ||
				((*tokens)->word->word && ft_strcmp((*tokens)->word->word,
							"&&") == 0))
			{
				parser_error(NULL, ERROR_PARSE, (*tokens)->word->word);
				dispose_words(words);
				dispose_redirects(redirects);
				return (NULL);
			}
			new_redirect = make_redirect(type, (*tokens)->word->word);
			if (new_redirect)
			{
				new_redirect->next = NULL;
				append_redirect(&redirects, new_redirect);
			}
			*tokens = (*tokens)->next;
		}
		else
		{
			word_copy = copy_word_desc(current->word);
			if (!word_copy)
			{
				dispose_words(words);
				dispose_redirects(redirects);
				return (NULL);
			}
			words = make_word_list(word_copy, words);
			if (!words)
			{
				dispose_word(word_copy);
				dispose_redirects(redirects);
				return (NULL);
			}
			*tokens = (*tokens)->next;
		}
	}
	return (make_simple_command(words, redirects));
}

static int	is_redirect_operator(const char *token)
{
	if (!token)
		return (0);
	if (ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0 ||
		ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0)
		return (1);
	return (0);
}

void	parser(t_command *command)
{
	t_word_list *word_list;
	t_command *ast;

	if (!command || !command->current_command)
		return ;
	command->type = CM_SIMPLE;
	command->simple = NULL;
	command->left = NULL;
	command->right = NULL;
	word_list = parse_command_line(command->current_command);
	if (!word_list)
		return ;
	ast = parse_tokens_to_ast(word_list);
	if (ast)
	{
		expand_ast(ast);
		command->type = ast->type;
		command->simple = ast->simple;
		command->left = ast->left;
		command->right = ast->right;
		ast->simple = NULL;
		ast->left = NULL;
		ast->right = NULL;
		dispose_ast_command(ast);
	}
	dispose_words(word_list);
}

static void	append_redirect(t_redirect **head, t_redirect *new_redirect)
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