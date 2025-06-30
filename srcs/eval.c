#include "eval.h"
#include "signal_handler.h"

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
