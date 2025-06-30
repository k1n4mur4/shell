#include "parser.h"
#include "error.h"

/* Static function declarations */
static t_command	*parse_pipeline(t_word_list **tokens);
static t_command	*parse_and_or(t_word_list **tokens);
static t_command	*parse_simple_command(t_word_list **tokens);
static int			is_redirect_operator(const char *token);

/* Debug function to print word list */
void	print_word_list(t_word_list *list)
{
	t_word_list	*current;
	int			index;

	index = 0;
	current = list;
	while (current)
	{
		ft_dprintf(STDOUT_FILENO, "Word[%d]: '%s' (flags: 0x%x)\n", 
			index++, current->word->word, current->word->flags);
		current = current->next;
	}
}

/* Debug function to print AST */
static void	print_ast(t_command *cmd, int depth)
{
	int	i;

	if (!cmd)
		return ;
	for (i = 0; i < depth; i++)
		ft_dprintf(STDOUT_FILENO, "  ");
	if (cmd->type == CM_SIMPLE)
	{
		t_redirect *redir;
		t_word_list *words;
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
}

/* Parse command line into word list */
t_word_list	*parse_command_line(const char *input)
{
	if (!input || !*input)
		return (NULL);
	return (lexer_tokenize(input));
}

/* Parse tokens into AST */
t_command	*parse_tokens_to_ast(t_word_list *tokens)
{
	t_word_list	*token_copy;

	if (!tokens)
		return (NULL);
	token_copy = tokens;
	return (parse_and_or(&token_copy));
}

/* Parse and/or operators (lowest precedence) */
static t_command	*parse_and_or(t_word_list **tokens)
{
	t_command	*left;
	t_command	*right;

	left = parse_pipeline(tokens);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->word->word && 
		   ft_strcmp((*tokens)->word->word, "&&") == 0)
	{
		*tokens = (*tokens)->next;
		
		/* Check for missing command after && operator */
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
		left = make_binary_command(CM_AND, left, right);
		if (!left)
		{
			dispose_ast_command(right);
			return (NULL);
		}
	}
	return (left);
}

/* Parse pipeline (higher precedence than &&) */
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
		
		/* Check for missing command after pipe */
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

/* Parse simple command with redirections */
static t_command	*parse_simple_command(t_word_list **tokens)
{
	t_word_list	*words;
	t_redirect	*redirects;
	t_word_list	*current;
	t_redirect_type	type;
	t_redirect	*new_redirect;

	if (!tokens || !*tokens)
		return (NULL);
	words = NULL;
	redirects = NULL;
	while (*tokens)
	{
		current = *tokens;
		if (current->word->flags & W_PIPE)
			break ;
		if (current->word->word && ft_strcmp(current->word->word, "&&") == 0)
			break ;
		if (is_redirect_operator(current->word->word))
		{
			/* Determine redirection type (check longer operators first) */
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
			
			/* Check for missing filename after redirection operator */
			if (!*tokens || !(*tokens)->word)
			{
				parser_error(NULL, ERROR_PARSE, "\\n");
				dispose_words(words);
				dispose_redirects(redirects);
				return (NULL);
			}
			
			/* Check for invalid redirection target (operators) */
			if (is_redirect_operator((*tokens)->word->word) || 
				((*tokens)->word->flags & W_PIPE) ||
				((*tokens)->word->word && ft_strcmp((*tokens)->word->word, "&&") == 0))
			{
				parser_error(NULL, ERROR_PARSE, (*tokens)->word->word);
				dispose_words(words);
				dispose_redirects(redirects);
				return (NULL);
			}
			
			new_redirect = make_redirect(type, (*tokens)->word->word);
			if (new_redirect)
			{
				new_redirect->next = redirects;
				redirects = new_redirect;
			}
			*tokens = (*tokens)->next;
		}
		else
		{
			t_word_desc *word_copy = copy_word_desc(current->word);
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

/* Check if token is redirection operator */
static int	is_redirect_operator(const char *token)
{
	if (!token)
		return (0);
	if (ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0 ||
		ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0)
		return (1);
	return (0);
}

/* Main parser function */
void	parser(t_command *command)
{
	t_word_list	*word_list;
	t_command	*ast;

	if (!command || !command->current_command)
		return ;
	ft_dprintf(STDOUT_FILENO, "Parsing: '%s'\n", command->current_command);
	word_list = parse_command_line(command->current_command);
	if (!word_list)
	{
		ft_dprintf(STDOUT_FILENO, "No tokens parsed\n");
		return ;
	}
	ft_dprintf(STDOUT_FILENO, "Tokens:\n");
	print_word_list(word_list);
	ast = parse_tokens_to_ast(word_list);
	if (ast)
	{
		ft_dprintf(STDOUT_FILENO, "AST before expansion:\n");
		print_ast(ast, 0);
		
		expand_ast(ast);
		
		ft_dprintf(STDOUT_FILENO, "AST after expansion:\n");
		print_ast(ast, 0);
		
		/* Copy AST structure to command for execution */
		command->type = ast->type;
		command->simple = ast->simple;
		command->left = ast->left;
		command->right = ast->right;
		/* Prevent double-free by clearing ast pointers */
		ast->simple = NULL;
		ast->left = NULL;
		ast->right = NULL;
		dispose_ast_command(ast);
	}
	else
		ft_dprintf(STDOUT_FILENO, "Failed to create AST\n");
	dispose_words(word_list);
}