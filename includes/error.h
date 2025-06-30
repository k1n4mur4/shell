#ifndef		ERROR_H
# define	ERROR_H

# include <stdarg.h>
# include "define.h"
# include "exit_value.h"

# include "ft_printf.h"

#define ERROR_SYNTAX "unexpected EOF while looking for matching `%c'\n"

void	parser_error(char *iname, const char *format, ...);

#endif