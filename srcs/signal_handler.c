#include "signal_handler.h"
#include "libft.h"
#include "ft_printf.h"
#include <unistd.h>

/* Global signal variable definition */
volatile sig_atomic_t	g_signal_received = 0;

/* Signal handler function */
void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_signal_received = SIGINT;
		ft_dprintf(STDOUT_FILENO, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
	{
		/* Do nothing for Ctrl+\ as per subject requirements */
		g_signal_received = SIGQUIT;
	}
}

/* Set up signal handlers */
void	set_signal(void)
{
	struct sigaction	sa;

	/* Initialize signal action structure */
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = signal_handler;

	/* Set SIGINT handler (Ctrl+C) */
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		ft_dprintf(STDERR_FILENO, ERROR_PREFIX "failed to set SIGINT handler\n");
		return ;
	}

	/* Set SIGQUIT handler (Ctrl+\) - do nothing */
	sa.sa_handler = signal_handler;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		ft_dprintf(STDERR_FILENO, ERROR_PREFIX "failed to set SIGQUIT handler\n");
		return ;
	}
}

/* Initialize shell signal handling */
void	shell_initialize(void)
{
	g_signal_received = 0;
	set_signal();
}

/* Setup signals for child processes (reset to default) */
void	setup_child_signals(void)
{
	struct sigaction	sa;

	/* Reset signals to default behavior in child processes */
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;

	/* Reset SIGINT to default (terminate process) */
	sigaction(SIGINT, &sa, NULL);
	
	/* Reset SIGQUIT to default (core dump) */
	sigaction(SIGQUIT, &sa, NULL);
}

/* Setup signals for parent process during command execution */
void	setup_parent_signals(void)
{
	struct sigaction	sa;

	/* Ignore signals in parent while child is running */
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;

	/* Ignore SIGINT while child is running */
	sigaction(SIGINT, &sa, NULL);
	
	/* Ignore SIGQUIT while child is running */
	sigaction(SIGQUIT, &sa, NULL);
}