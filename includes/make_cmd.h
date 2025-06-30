#ifndef MAKE_CMD_H
# define MAKE_CMD_H

# include "command.h"
# include "libft.h"
# include "ft_printf.h"

/* Word flags for t_word_desc */
# define W_HASDOLLAR	0x0001	/* Word contains $ */
# define W_QUOTED		0x0002	/* Word is quoted */
# define W_PIPE			0x0004	/* Word is | */
# define W_ASSIGNMENT	0x0008	/* Word is assignment */
# define W_GLOBPAT		0x0010	/* Word is glob pattern */
# define W_NOGLOB		0x0020	/* No glob expansion */
# define W_AND			0x0040	/* Word is && */
# define W_OR			0x0080	/* Word is || */

/* Word creation functions */
t_word_desc		*alloc_word_desc(void);
t_word_desc		*make_bare_word(const char *string);
t_word_desc		*make_word_flags(t_word_desc *w, const char *string);
t_word_desc		*make_word(const char *string);
t_word_desc		*copy_word_desc(const t_word_desc *original);
t_word_list		*make_word_list(t_word_desc *word, t_word_list *wlink);

/* Word manipulation functions */
void			dispose_word(t_word_desc *w);
void			dispose_words(t_word_list *list);
t_word_list		*copy_word_list(t_word_list *list);

/* Command creation functions */
t_command		*alloc_command(void);
t_simple_command *alloc_simple_command(void);
t_redirect		*alloc_redirect(void);
t_command		*make_simple_command(t_word_list *words, t_redirect *redirects);
t_command		*make_binary_command(t_command_type type, t_command *left, t_command *right);
t_redirect		*make_redirect(t_redirect_type type, const char *filename);

/* Command manipulation functions */
void			dispose_ast_command(t_command *cmd);
void			dispose_redirects(t_redirect *redirects);

#endif