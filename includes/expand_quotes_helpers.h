/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes_helpers.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 04:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_QUOTES_HELPERS_H
# define EXPAND_QUOTES_HELPERS_H

// expand_quotes_helpers.c
char	*get_variable_value(char *var_name);
char	*append_variable(char *result, const char *word, int *i);
char	*append_literal(char *result, const char *word, int *i);
char	*process_double_quoted_char(char *result, const char *word, int *i);
char	*process_unquoted_char(char *result, const char *word, int *i);
char	*process_single_quote(const char *word, int *i);
char	*process_double_quote(const char *word, int *i);
char	*process_unquoted(const char *word, int *i);
char	*append_to_result(char *result, char *temp);

#endif