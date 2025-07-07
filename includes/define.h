#ifndef DEFINE_H
#define DEFINE_H

#include <signal.h>

#define ENAME	"minishell"

#define	PROMPT	ENAME "$ "

/* Error message prefixes */
#define ERROR_PREFIX		ENAME ": "
#define ERROR_SYNTAX_PREFIX	ENAME ": syntax error: "
#define ERROR_COMMAND_PREFIX	ENAME ": command not found: "
#define ERROR_PERMISSION_PREFIX	ENAME ": permission denied: "
#define ERROR_NO_SUCH_FILE	ENAME ": no such file or directory: "


typedef enum e_type
{
	SET,
	GET,
	SHOW,
	ADD,
	FREE,
	RESET
}				t_type;

#endif