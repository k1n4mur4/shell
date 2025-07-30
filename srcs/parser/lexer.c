/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:38 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:37:03 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "parser.h"

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}

static void	skip_whitespace(const char **input)
{
	while (**input && is_whitespace(**input))
		(*input)++;
}

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

static int	is_word_char(char c)
{
	return (!is_whitespace(c) && !is_operator_char(c));
}

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

static char	*init_word_result(void)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	return (result);
}

static char	*process_word_part(const char **input, char *result)
{
	char	*part;
	char	*tmp;

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
		return (result);
	}
	tmp = append_string(result, part);
	free(result);
	free(part);
	return (tmp);
}

static char	*extract_complete_word(const char **input)
{
	char	*result;

	result = init_word_result();
	if (!result)
		return (NULL);
	while (**input && is_word_char(**input))
	{
		result = process_word_part(input, result);
		if (!result)
			return (NULL);
	}
	if (result && result[0] == '\0')
	{
		free(result);
		return (NULL);
	}
	return (result);
}

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

static void	update_quote_state(char c, int *in_squote, int *in_dquote)
{
	if (c == '\'' && !*in_dquote)
		*in_squote = !*in_squote;
	else if (c == '"' && !*in_squote)
		*in_dquote = !*in_dquote;
}

static int	check_quote_errors(int in_squote, int in_dquote)
{
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

static int	validate_input(const char *input)
{
	int	in_squote;
	int	in_dquote;

	in_squote = 0;
	in_dquote = 0;
	while (*input)
	{
		update_quote_state(*input, &in_squote, &in_dquote);
		input++;
	}
	return (check_quote_errors(in_squote, in_dquote));
}

static t_word_list	*process_single_token(const char **current,
											t_word_list *word_list)
{
	t_word_desc	*word_desc;
	char		*token;
	t_word_list	*new_word_list;

	token = get_next_token(current);
	if (!token)
		return (word_list);
	word_desc = make_word(token);
	free(token);
	if (!word_desc)
	{
		dispose_words(word_list);
		return (NULL);
	}
	{
		new_word_list = make_word_list(word_desc, word_list);
		if (!new_word_list)
		{
			dispose_word(word_desc);
			dispose_words(word_list);
			return (NULL);
		}
		word_list = new_word_list;
	}
	return (word_list);
}

t_word_list	*lexer_tokenize(const char *input)
{
	t_word_list *word_list;
	const char *current;

	if (!input)
		return (NULL);
	if (!validate_input(input))
		return (NULL);
	word_list = NULL;
	current = input;
	while (*current)
	{
		word_list = process_single_token(&current, word_list);
		if (!word_list && *current)
			return (NULL);
		if (!*current)
			break ;
	}
	return (word_list);
}