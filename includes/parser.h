#ifndef	PARSER_H
# define PARSER_H

#include "command.h"
#include "make_cmd.h"
#include "ft_printf.h"

/* Lexer functions */
t_word_list		*lexer_tokenize(const char *input);

/* Parser functions */
void			parser(t_command *command);
t_word_list		*parse_command_line(const char *input);
void			print_word_list(t_word_list *list);
t_command		*parse_tokens_to_ast(t_word_list *tokens);

/* Expansion functions */
void			expand_ast(t_command *ast);
char			*expand_word(const char *word);
int				should_expand_in_context(const char *word);

#endif