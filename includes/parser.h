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

int			is_whitespace(char c);
int			is_operator_char(char c);
void		lexer_skip_whitespace(const char **input);
int			is_word_char(char c);
char		*append_string(char *base, char *append);
char		*extract_quoted_string(const char **input, char quote);
char		*extract_operator(const char **input);
char		*extract_word(const char **input);
char		*init_word_result(void);
char		*extract_complete_word(const char **input);
void		update_quote_state(char c, int *in_squote, int *in_dquote);
int			check_quote_errors(int in_squote, int in_dquote);
int			validate_input(const char *input);
char		*process_word_part(const char **input, char *result);
char		*get_next_token(const char **input);
t_word_list	*process_single_token(const char **current, t_word_list *word_list);

void		parser(t_command *command);
t_word_list	*parse_command_line(const char *input);
void		print_word_list(t_word_list *list);
t_command	*parse_tokens_to_ast(t_word_list *tokens);

void		print_ast(t_command *cmd, int depth);
t_command	*parse_and_or(t_word_list **tokens);
t_command	*parse_pipeline(t_word_list **tokens);
t_command	*parse_simple_command(t_word_list **tokens);
int			is_redirect_operator(const char *token);
void		append_redirect(t_redirect **head, t_redirect *new_redirect);

void		expand_ast(t_command *ast);
char		*expand_word(const char *word);
char		*expand_word_with_flags(const char *word, int flags);
int			should_expand_in_context(const char *word);
char		*expand_single_quoted(const char *word);
char		*expand_double_quoted(const char *word);
char		*expand_unquoted(const char *word);
char		*expand_mixed_quotes(const char *word);
char		*extract_var_name(const char *str, int *advance);
char		*get_env_value(const char *var_name);
char		*get_special_var(char c);

#endif