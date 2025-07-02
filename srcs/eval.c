#include "eval.h"
#include "signal_handler.h"

/* Shell termination flag management */
static int	g_shell_exit_flag = 0;

void	set_shell_exit_flag(int flag)
{
	g_shell_exit_flag = flag;
}

int	should_shell_exit(void)
{
	return (g_shell_exit_flag);
}

/*
 * minishellのメインREPL（Read-Eval-Print Loop）
 * 
 * この関数が処理する内容:
 * - シグナル管理（SIGINTを処理、SIGQUITは完全に無視）
 * - readlineによるユーザー入力の読み取り
 * - コマンドの解析と実行
 * - 入力とコマンド構造体のメモリ管理
 * - 適切なクリーンアップとエラーハンドリング
 * 
 * 戻り値: 最後に実行されたコマンドの終了ステータス
 */
int	reader_loop(void)
{
	t_command			command;

	/* インタラクティブモード用のシグナルハンドラーを初期化 */
	set_signal();
	while (1)
	{
		/* Check for exit flag before reading new command */
		if (should_shell_exit())
			break ;
			
		command.current_command = readline(ft_strjoin(ENAME, "$ "));
		/* Ctrl+D（EOF）を処理 - ユーザーが終了を要求 */
		if (!command.current_command)
		{
			write(2, "exit\n", 5);
			break ;
		}
		/* 空のコマンドをスキップ（空白のみまたは空の入力） */
		if (!*(command.current_command))
		{
			free(command.current_command);
			continue ;
		}
		/* 有効なコマンドを処理: 解析、実行、クリーンアップ */
		set_deftext(command.current_command);  /* 履歴に追加 */
		parser(&command);                      /* ASTに解析 */
		execute_command(&command);             /* 解析されたコマンドを実行 */
		dispose_command(&command);             /* メモリをクリーンアップ */
	}
	return (exit_value(0, GET));
}
