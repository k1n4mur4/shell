/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes_helpers2.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 04:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_QUOTES_HELPERS2_H
# define EXPAND_QUOTES_HELPERS2_H

# include "command.h"

char	*process_single_quote(const char *word, int *i);
char	*process_double_quote(const char *word, int *i);
char	*process_unquoted(const char *word, int *i);
char	*append_to_result(char *result, char *temp);
int		process_word(t_word_list **tokens, t_word_list **words,
			t_redirect **redirects);

#endif