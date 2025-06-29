# Minishell

## Summary
このプロジェクトでは、シンプルなシェルを作成します。

そう、自分だけの小さなBashです。
プロセスやファイルディスクリプタについての深い知識を得ることができます。

バージョン: 8.02

---

## 目次
1. [Introduction](#introduction)
2. [Common Instructions](#common-instructions)
3. [Mandatory Part](#mandatory-part)
4. [Bonus Part](#bonus-part)
5. [Submission and Peer-Evaluation](#submission-and-peer-evaluation)

---

## Introduction
シェルはITの黎明期から存在しています。
当時、開発者たちは1/0スイッチを操作してコンピュータとやり取りすることに非常に不満を感じていました。
そこで、より直感的な方法として、対話型コマンドラインを用いてコンピュータとコミュニケーションを取るソフトウェアが開発されました。

Minishellでは、Windowsが登場する前の開発者たちが直面した課題を体験することができます。

---

## Common Instructions
- プロジェクトはC言語で記述すること。
- コードはNorm規約に従うこと。
- すべてのヒープメモリは適切に解放すること。メモリリークは禁止。
- プログラムが予期せずクラッシュしないこと（セグメンテーションフォルト、バスエラー、二重解放など）。
- Makefileを作成し、適切なルール（`NAME, all, clean, fclean, re`）を含めること。
- ボーナス部分は`bonus`ルールをMakefileに含めること。
- `libft`の使用が許可されている場合は、`libft`フォルダ内にソースコードとMakefileを配置すること。
- 提出物は指定されたGitリポジトリにアップロードすること。

---

## Mandatory Part
**プログラム名:** `minishell`

**提出ファイル:** `Makefile, *.h, *.c`

**Makefileルール:** `NAME, all, clean, fclean, re`

**使用可能な外部関数:**
`readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history, printf, malloc, free, write, access, open, read, close, fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sigaddset, kill, exit, getcwd, chdir, stat, lstat, fstat, unlink, execve, dup, dup2, pipe, opendir, readdir, closedir, strerror, perror, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs`

### 機能要件
- コマンド入力待機中にプロンプトを表示。
- コマンド履歴の実装。
- `PATH` 環境変数を考慮したコマンド実行。
- シグナル受信時の挙動管理（グローバル変数は1つまで）。
- `|`（パイプ）の実装。
- 環境変数の展開（`$VAR`）。
- `$?`（直前の終了ステータス）の処理。
- **リダイレクトの実装:**
  - `<` 標準入力リダイレクト。
  - `>` 標準出力リダイレクト。
  - `<<` ヒアドキュメント。
  - `>>` 追記モードリダイレクト。
- **シェルの組み込みコマンド:**
  - `echo`（`-n` オプション対応）
  - `cd`（相対パス・絶対パスのみ）
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- **シグナル対応:**
  - `Ctrl+C` 新しいプロンプト表示。
  - `Ctrl+D` シェルの終了。
  - `Ctrl+\` 何もしない。

**注意:** `readline()`関数自体が発生させるメモリリークは修正不要。ただし、自作コードのメモリリークは許容されない。

---

## Bonus Part
**ボーナス要件:**
- `&&` および `||` 演算子の実装（優先順位に括弧を適用）。
- `*`（ワイルドカード）の実装（カレントディレクトリでの展開）。

**評価基準:**
- ボーナス部分は、必須部分が完全に動作する場合のみ評価対象となる。

---

## Submission and Peer-Evaluation
- Gitリポジトリへの適切な提出。
- 必須部分の要件をすべて満たしていることを確認すること。
- 提出ファイル名の正確性を確認すること。

---

Minishellプロジェクトでは、Bashの基本機能を模倣しながら、プロセス管理、シグナル処理、リダイレクト、環境変数の展開などを学びます。しっかりと実装し、デバッグを行いながら、シェルプログラミングの理解を深めていきましょう！
