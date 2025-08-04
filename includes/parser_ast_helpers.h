/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_helpers.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 04:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_AST_HELPERS_H
# define PARSER_AST_HELPERS_H

# include "command.h"

// parser_ast_helpers.c
t_command		*handle_missing_right_operand(t_command *left);
t_command		*create_binary_command(t_command *left, t_command *right,
					t_command_type type);
t_redirect_type	get_redirect_type(const char *op);
int				validate_redirect_target(t_word_list **tokens, const char *op);
int				process_redirect(t_word_list **tokens, t_redirect **redirects);

#endif