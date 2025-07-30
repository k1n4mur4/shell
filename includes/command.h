/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:36:26 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 02:18:52 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

typedef struct	s_word_desc
{
	char	*word;
	int		flags;
}						t_word_desc;

typedef struct	s_word_list
{
	struct s_word_list	*next;
	t_word_desc			*word;
}						t_word_list;

typedef enum	e_command_type
{
	CM_SIMPLE,
	CM_PIPE,
	CM_AND,
	CM_OR
}						t_command_type;

typedef enum	e_redirect_type
{
	R_INPUT,
	R_OUTPUT,
	R_APPEND,
	R_HEREDOC
}						t_redirect_type;

typedef struct	s_redirect
{
	t_redirect_type		type;
	char				*filename;
	struct s_redirect	*next;
}						t_redirect;

typedef struct	s_simple_command
{
	t_word_list			*words;
	t_redirect			*redirects;
}						t_simple_command;

typedef struct	s_command
{
	t_command_type		type;
	t_simple_command	*simple;
	struct s_command	*left;
	struct s_command	*right;
	char				*current_command;
}						t_command;

#endif