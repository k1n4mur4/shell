#include "redirect_utils.h"

void	cleanup_temp_file(const char *temp_filename)
{
	if (temp_filename)
		unlink(temp_filename);
}