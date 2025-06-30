#include "exit_value.h"

int	exit_value(int exit_value, enum e_type type)
{
	static int	last_command_exit_value = 0;

	if (type == SET)
	{
		last_command_exit_value = exit_value;
	}
	else if (type == GET)
	{
		exit_value = last_command_exit_value;
	}
	return (exit_value);
}
