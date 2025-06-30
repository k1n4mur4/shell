#ifndef REDIRECT_UTILS_H
#define REDIRECT_UTILS_H

#include "command.h"
#include "libft.h"
#include "ft_printf.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <readline/readline.h>

/* Redirection backup structure */
typedef struct s_redirect_backup
{
	int	stdin_backup;
	int	stdout_backup;
	int	stderr_backup;
}	t_redirect_backup;

/* Redirection functions */
int					setup_redirections(t_redirect *redirects, t_redirect_backup *backup);
void				restore_redirections(t_redirect_backup *backup);
int					handle_input_redirect(const char *filename);
int					handle_output_redirect(const char *filename);
int					handle_append_redirect(const char *filename);
int					handle_heredoc_redirect(const char *delimiter);

/* Helper functions */
void				init_redirect_backup(t_redirect_backup *backup);
void				print_redirect_error(const char *filename, const char *error_msg);

#endif