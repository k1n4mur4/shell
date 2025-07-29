/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:37:02 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 01:37:04 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "command.h"
# include "ft_printf.h"
# include "make_cmd.h"

t_word_list	*lexer_tokenize(const char *input);

void		parser(t_command *command);
t_word_list	*parse_command_line(const char *input);
void		print_word_list(t_word_list *list);
t_command	*parse_tokens_to_ast(t_word_list *tokens);

void		expand_ast(t_command *ast);
char		*expand_word(const char *word);
int			should_expand_in_context(const char *word);
char		*expand_single_quoted(const char *word);
char		*expand_double_quoted(const char *word);
char		*expand_unquoted(const char *word);
char		*expand_mixed_quotes(const char *word);
char		*extract_var_name(const char *str, int *advance);
char		*get_env_value(const char *var_name);
char		*get_special_var(char c);

#endif