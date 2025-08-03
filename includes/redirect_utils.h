/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:37:12 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:16:07 by kinamura         ###   ########.fr       */
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

/* Heredoc context structure */
typedef struct s_heredoc_context
{
	char	*tempfile;
	int		fd;
	char	*delimiter;
}	t_heredoc_context;

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

typedef struct s_string_append_params
{
	char	*str;
	char	**s1;
	char	*s2;
	size_t	s1_len;
	size_t	s2_len;
}						t_string_append_params;

/* Heredoc context management */
void					set_heredoc_context(t_heredoc_context *context);
t_heredoc_context		*get_heredoc_context(void);
int						setup_redirections(t_redirect *redirects,
							t_redirect_backup *backup);
void					restore_redirections(t_redirect_backup *backup);
int						handle_input_redirect(const char *filename);
int						handle_output_redirect(const char *filename);
int						handle_append_redirect(const char *filename);
int						handle_heredoc_redirect(const char *delimiter);
void					init_redirect_backup(t_redirect_backup *backup);
void					print_redirect_error(const char *filename,
							const char *error_msg);
void					cleanup_temp_file(char *template);
char					*ft_strappend(char **s1, char *s2);
char					*read_heredoc_content(const char *delimiter);
void					heredoc_signal_handler(int signum);
int						setup_heredoc_signals(struct sigaction *old_sa);
int						create_heredoc_temp_file(char **template,
							t_heredoc_context *context);
int						write_heredoc_content_to_file(int fd,
							const char *delimiter);

#endif