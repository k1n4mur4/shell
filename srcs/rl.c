#include "rl.h"

void	set_deftext(char *deftext)
{
	if (!deftext)
		return ;
	add_history(deftext);
}