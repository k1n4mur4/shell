#ifndef PATH_UTILS_H
#define PATH_UTILS_H

#include "env.h"
#include "libft.h"
#include <unistd.h>
#include <sys/stat.h>

/* Path search functions */
char	*find_command_path(const char *command);
int		is_executable(const char *path);
char	**get_path_dirs(void);

#endif