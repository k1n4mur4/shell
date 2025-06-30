#include "parser.h"
#include "error.h"

/* Check if character is whitespace */
static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

/* Check if character is special operator */
static int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}

/* Skip whitespace in input */
static void	skip_whitespace(const char **input)
{
	while (**input && is_whitespace(**input))
		(*input)++;
}

/* Extract quoted string with quotes preserved */
static char	*extract_quoted_string(const char **input, char quote)
{
	const char	*start;
	int			len;
	char		*result;

	start = *input;
	(*input)++;
	while (**input && **input != quote)
		(*input)++;
	if (**input != quote)
		return (NULL);
	(*input)++;
	len = *input - start;
	result = ft_calloc(len + 1, sizeof(char));
	if (!result)
		return (NULL);
	ft_strlcpy(result, start, len + 1);
	return (result);
}

/* Extract operator token */
static char	*extract_operator(const char **input)
{
	const char	*start;
	char		*result;
	int			len;

	start = *input;
	if ((**input == '<' || **input == '>') && *(*input + 1) == **input)
		*input += 2;
	else if (**input == '&' && *(*input + 1) == '&')
		*input += 2;
	else if (**input == '|' && *(*input + 1) == '|')
		*input += 2;
	else
		(*input)++;
	len = *input - start;
	result = ft_calloc(len + 1, sizeof(char));
	if (!result)
		return (NULL);
	ft_strlcpy(result, start, len + 1);
	return (result);
}

/* Extract regular word */
static char	*extract_word(const char **input)
{
	const char	*start;
	char		*result;
	int			len;

	start = *input;
	while (**input && !is_whitespace(**input) && 
		   !is_operator_char(**input) && **input != '\'' && **input != '"')
		(*input)++;
	len = *input - start;
	result = ft_calloc(len + 1, sizeof(char));
	if (!result)
		return (NULL);
	ft_strlcpy(result, start, len + 1);
	return (result);
}

/* Check if character can be part of a word token */
static int	is_word_char(char c)
{
	return (!is_whitespace(c) && !is_operator_char(c));
}

/* Helper function to append string */
static char	*append_string(char *base, char *append)
{
	char	*new_str;
	size_t	base_len;
	size_t	append_len;
	size_t	new_len;

	if (!base || !append)
		return (NULL);
	base_len = ft_strlen(base);
	append_len = ft_strlen(append);
	new_len = base_len + append_len + 1;
	new_str = ft_calloc(new_len, sizeof(char));
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, base, new_len);
	ft_strlcat(new_str, append, new_len);
	return (new_str);
}

/* Extract complete word including consecutive quotes and regular chars */
static char	*extract_complete_word(const char **input)
{
	char	*result;
	char	*part;
	char	*tmp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (**input && is_word_char(**input))
	{
		if (**input == '\'' || **input == '"')
			part = extract_quoted_string(input, **input);
		else
			part = extract_word(input);
		if (!part)
		{
			if (result[0] == '\0')
			{
				free(result);
				return (NULL);
			}
			break ;
		}
		tmp = append_string(result, part);
		free(result);
		free(part);
		if (!tmp)
		{
			return (NULL);
		}
		result = tmp;
	}
	if (result && result[0] == '\0')
	{
		free(result);
		return (NULL);
	}
	return (result);
}

/* Get next token from input */
static char	*get_next_token(const char **input)
{
	skip_whitespace(input);
	if (!**input)
		return (NULL);
	if (is_operator_char(**input))
		return (extract_operator(input));
	else
		return (extract_complete_word(input));
}

/* Pre-validate input for syntax errors */
static int	validate_input(const char *input)
{
	int		in_squote;
	int		in_dquote;

	in_squote = 0;
	in_dquote = 0;
	while (*input)
	{
		if (*input == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (*input == '"' && !in_squote)
			in_dquote = !in_dquote;
		input++;
	}
	if (in_squote)
	{
		parser_error(NULL, ERROR_SYNTAX, '\'');
		return (0);
	}
	if (in_dquote)
	{
		parser_error(NULL, ERROR_SYNTAX, '"');
		return (0);
	}
	return (1);
}

/* Main lexer function - tokenize input string into word list */
t_word_list	*lexer_tokenize(const char *input)
{
	t_word_list	*word_list;
	t_word_desc	*word_desc;
	char		*token;
	const char	*current;

	if (!input)
		return (NULL);
	if (!validate_input(input))
		return (NULL);
	word_list = NULL;
	current = input;
	while (*current)
	{
		token = get_next_token(&current);
		if (!token)
			break ;
		word_desc = make_word(token);
		free(token);
		if (!word_desc)
		{
			dispose_words(word_list);
			return (NULL);
		}
		word_list = make_word_list(word_desc, word_list);
		if (!word_list)
		{
			dispose_word(word_desc);
			return (NULL);
		}
	}
	return (word_list);
}