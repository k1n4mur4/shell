#include "make_cmd.h"

/* Free a single word descriptor */
void	dispose_word(t_word_desc *w)
{
	if (!w)
		return ;
	if (w->word)
		free(w->word);
	free(w);
}

/* Free a word list */
void	dispose_words(t_word_list *list)
{
	t_word_list	*temp;

	while (list)
	{
		temp = list;
		list = list->next;
		dispose_word(temp->word);
		free(temp);
	}
}

/* Copy a word list */
t_word_list	*copy_word_list(t_word_list *list)
{
	t_word_list	*new_list;
	t_word_list	*current;
	t_word_list	*new_node;
	t_word_desc	*new_word;

	new_list = NULL;
	current = list;
	while (current)
	{
		new_word = make_word(current->word->word);
		if (!new_word)
		{
			dispose_words(new_list);
			return (NULL);
		}
		new_word->flags = current->word->flags;
		new_node = make_word_list(new_word, new_list);
		if (!new_node)
		{
			dispose_word(new_word);
			dispose_words(new_list);
			return (NULL);
		}
		if (!new_list)
			new_list = new_node;
		current = current->next;
	}
	return (new_list);
}