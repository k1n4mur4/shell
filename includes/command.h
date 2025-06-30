#ifndef	COMMAND_H
#define COMMAND_H

/* A structure which represents a word. */
typedef struct s_word_desc
{
	char		*word;		/* Zero terminated string. */
	int			flags;		/* Flags associated with this word. */
}				t_word_desc;

/* A linked list of words. */
typedef struct s_word_list
{
	struct s_word_list	*next;
	t_word_desc			*word;
}				t_word_list;

typedef struct s_command
{
	char	*current_command;
}				t_command;

#endif