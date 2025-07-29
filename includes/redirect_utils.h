/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:37:12 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 01:37:13 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_UTILS_H
# define REDIRECT_UTILS_H

# include "command.h"
# include "ft_printf.h"
# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <unistd.h>

/* Redirection backup structure */
typedef struct s_redirect_backup
{
	int					stdin_backup;
	int					stdout_backup;
	int					stderr_backup;
}						t_redirect_backup;

/* Temporary file list structure */
typedef struct s_temp_file
{
	char				*filename;
	struct s_temp_file	*next;
}						t_temp_file;

/* Redirection functions */
int						setup_redirections(t_redirect *redirects,
							t_redirect_backup *backup);
void					restore_redirections(t_redirect_backup *backup);
int						handle_input_redirect(const char *filename);
int						handle_output_redirect(const char *filename);
int						handle_append_redirect(const char *filename);
int						handle_heredoc_redirect(const char *delimiter);

/* Helper functions */
void					init_redirect_backup(t_redirect_backup *backup);
void					print_redirect_error(const char *filename,
							const char *error_msg);

/* Temporary file functions for heredoc */
void					cleanup_temp_file(char *template);

#endif