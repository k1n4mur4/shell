#include "eval.h"
#include "signal_handler.h"
#include "make_cmd.h"
#include "exit_value.h"

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
		/* Initialize command structure before use */
		command.type = CM_SIMPLE;
		command.simple = NULL;
		command.left = NULL;
		command.right = NULL;
		command.current_command = readline(PROMPT);
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
		dispose_current_command(&command);     /* current_commandをクリーンアップ */
		
		/* Check if exit command was executed */
		if (shell_exit_status(0, GET) >= 0)  /* Shell exit requested */
		{
			return (shell_exit_status(0, GET));
		}
	}
	return (exit_value(0, GET));
}
