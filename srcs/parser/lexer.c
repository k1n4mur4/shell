/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:38 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 20:52:27 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "parser.h"

t_word_list	*lexer_tokenize(const char *input)
{
	t_word_list	*word_list;
	const char	*current;

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
