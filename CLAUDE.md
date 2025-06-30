# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## プロジェクト概要
42スクールのMinishellプロジェクト - Bashライクなシェルの実装（C言語、Norm規約準拠）

最初に全てのファイルを確認してください。
詳細な課題要件については`subject.md`を参照してください。

## ビルドコマンド

```bash
make          # minishellをビルド（libftも含む）
make clean    # オブジェクトファイルを削除
make fclean   # 実行ファイルとオブジェクトファイルを削除
make re       # 完全な再ビルド（fclean + all）
make run      # minishellを実行
make val      # valgrindでメモリリーク検査を実行
make test     # minishellをテスト実行（エラー出力を抑制）
```

## プロジェクトアーキテクチャ

### 環境変数管理システム
- `t_env`構造体でリンクリスト形式で環境変数を管理
- `env()`関数で静的変数を使ったシングルトンパターンを実装
- 操作タイプは`t_type`エナム（SET、GET、SHOW、ADD、FREE、RESET）で管理

### libft統合
- 独自のlibftライブラリを使用（標準Cライブラリ関数の再実装）
- OS依存部分（Mac/Linux）はMakefileで自動切り替え
- readline関連のパスも自動設定（Homebrew/システム）

### 重要な実装パターン
1. **静的変数によるグローバルステート管理**
   - `env()`関数内で環境変数リストを保持
   - `exit_value()`関数内で最後のコマンドの終了値を保持

2. **メモリ管理**
   - 全ての動的メモリはlibftのft_calloc/freeで管理
   - readline()の戻り値は必ずfree()が必要
   - rl_clear_history()を終了時に実行

3. **シグナル処理**
   - グローバル変数は1つまで（`volatile sig_atomic_t g_signal_received`）
   - sigaction()を使用してSIGINT、SIGQUITを処理
   - readlineとの統合（rl_on_new_line、rl_replace_line、rl_redisplay）

4. **エラーハンドリング**
   - ft_dprintfでSTDERR_FILENOにエラー出力
   - 終了値は127（コマンドが見つからない）等の標準値を使用

## 実装進捗と構造

### 現在実装済み
- 基本的なREPLループ（`reader_loop()`）
- readlineによる入力受付とプロンプト表示
- コマンド履歴機能
- 環境変数管理システム
- 終了値管理システム
- "exit"コマンドの基本実装

### 実装予定（subject.mdの要件）
- シグナルハンドリング（Ctrl+C、Ctrl+D、Ctrl+\）
- レクサー・パーサー
- コマンド実行（PATH考慮）
- パイプ（`|`）、リダイレクト（`<`、`>`、`<<`、`>>`）
- 環境変数展開（`$VAR`、`$?`）
- ビルトインコマンド（echo、cd、pwd、export、unset、env、exit）

## 開発ガイドライン

### gitflow
- developブランチから新機能を開発
- 新機能はfeature/ブランチとして作成  
- mainブランチへの直接PRは禁止

### 計画管理
- 実装計画は`plan.md`に記載
- 詳細な計画は`phase1_plan.md`等のフェーズ別ファイルに分割

### メモリリーク対策
- readlineライブラリ自体のメモリリークは許容
- 自作コードのメモリリークは完全に修正が必要
- valgrindで定期的に検査

## 開発時の注意点
- Norm規約の厳守
- 使用可能な外部関数のみ使用（subject.mdに記載）
- シグナル処理でのグローバル変数は1つまで制限
- 全てのヒープメモリの適切な解放

## 追加指示
- 日本語で返答してください
- gitflowを用いて開発を行なってください。新機能を追加する場合はdevelopブランチから派生させてfeatureブランチとしてください
- mainブランチにはPRを出さないでください
- 計画は常にplan.mdに出力して、plan.mdを書き換える様にしてください