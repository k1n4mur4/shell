#ifndef REDIRECT_UTILS_H
#define REDIRECT_UTILS_H

#include "command.h"
#include "libft.h"
#include "ft_printf.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <readline/readline.h>

/* Redirection backup structure */
typedef struct s_redirect_backup
{
	int	stdin_backup;
	int	stdout_backup;
	int	stderr_backup;
}	t_redirect_backup;

/* Temporary file list structure */
typedef struct s_temp_file
{
	char				*filename;
	struct s_temp_file	*next;
}	t_temp_file;

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

/* Temporary file functions for heredoc */
char				*create_temp_file(void);
int					write_heredoc_to_temp(const char *delimiter, const char *temp_filename);
void				cleanup_temp_file(const char *temp_filename);

/* Temporary file list management */
void				add_temp_file(const char *filename);
void				remove_temp_file(const char *filename);
void				cleanup_all_temp_files(void);
void				init_temp_file_cleanup(void);

#endif