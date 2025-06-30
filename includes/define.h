#ifndef DEFINE_H
#define DEFINE_H

#include <signal.h>

#define ENAME	"minishell"

/* Global signal variable (42 project allows only one global variable) */
extern volatile sig_atomic_t	g_signal_received;

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