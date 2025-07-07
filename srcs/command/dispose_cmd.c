#include "dispose_cmd.h"
#include "make_cmd.h"

void	dispose_current_command(t_command	*command)
{
	if (!command)
		return;
	
	/* Free AST components without freeing the command structure itself */
	if (command->simple)
	{
		dispose_words(command->simple->words);
		dispose_redirects(command->simple->redirects);
		free(command->simple);
		command->simple = NULL;
	}
	if (command->left)
	{
		dispose_ast_command(command->left);
		command->left = NULL;
	}
	if (command->right)
	{
		dispose_ast_command(command->right);
		command->right = NULL;
	}
	if (command->current_command)
	{
		free(command->current_command);
		command->current_command = NULL;
	}
}
