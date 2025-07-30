/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:38 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:37:03 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "parser.h"

void	update_quote_state(char c, int *in_squote, int *in_dquote)
{
	if (c == '\'' && !*in_dquote)
		*in_squote = !*in_squote;
	else if (c == '"' && !*in_squote)
		*in_dquote = !*in_dquote;
}

int	check_quote_errors(int in_squote, int in_dquote)
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

int	validate_input(const char *input)
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