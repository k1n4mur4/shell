#include "dispose_cmd.h"

void	dispose_command(t_command	*command)
{
	if (command)
	{
		if (command->current_command)
			free(command->current_command);
	}
}
