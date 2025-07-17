#include "signal_handler.h"
#include "redirect_utils.h"
#include "libft.h"
#include "ft_printf.h"
#include <unistd.h>

/* シグナルハンドラー関数 */
void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		/* 改行を出力してreadlineを新しい行に設定 */
		ft_dprintf(STDOUT_FILENO, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	/* SIGQUITはset_signal()でSIG_IGNにより完全に無視される */
}

/* シグナルハンドラーを設定 */
void	set_signal(void)
{
	struct sigaction	sa;

	/* シグナルアクション構造体を初期化 */
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = signal_handler;

	/* SIGINTハンドラーを設定（Ctrl+C） */
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		ft_dprintf(STDERR_FILENO, ERROR_PREFIX "failed to set SIGINT handler\n");
		return ;
	}

	/* SIGQUITハンドラーを設定（Ctrl+\） - 完全に無視 */
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		ft_dprintf(STDERR_FILENO, ERROR_PREFIX "failed to set SIGQUIT handler\n");
		return ;
	}
}

/* シェルのシグナル処理を初期化 */
void	shell_initialize(void)
{
	set_signal();
}

/* 子プロセス用のシグナル設定（デフォルトにリセット） */
void	setup_child_signals(void)
{
	struct sigaction	sa;

	/* 子プロセスでシグナルをデフォルト動作にリセット */
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;

	/* SIGINTをデフォルトにリセット（プロセス終了） */
	sigaction(SIGINT, &sa, NULL);
	
	/* SIGQUITをデフォルトにリセット（コアダンプ） */
	sigaction(SIGQUIT, &sa, NULL);
}

/* コマンド実行中の親プロセス用シグナル設定 */
void	setup_parent_signals(void)
{
	struct sigaction	sa;

	/* 子プロセス実行中は親プロセスでシグナルを無視 */
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;

	/* 子プロセス実行中はSIGINTを無視 */
	sigaction(SIGINT, &sa, NULL);
	
	/* 子プロセス実行中はSIGQUITを無視 */
	sigaction(SIGQUIT, &sa, NULL);
}