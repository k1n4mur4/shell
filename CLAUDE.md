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

## コアアーキテクチャ詳細

### 主要データフロー
```
main() → shell() → reader_loop() → [parser() → execute_command()]
                ↓
         initialize_environment() → env(SET)
```

### 重要な実装状況
**完全実装済み**:
- `env.c`: 環境変数管理（リンクリスト、シングルトンパターン）
- `exit_value.c`: 終了ステータス管理
- `eval.c`: REPLループとreadline統合
- `dispose_cmd.c`: コマンド構造体のメモリ解放

**スタブ状態（要実装）**:
- `parser/parser.c`: 構文解析（現在空関数）
- `execute/execute_cmd.c`: コマンド実行（現在空関数）

### データ構造の継承関係
```c
// command.h - 基本構造（現状シンプル）
typedef struct s_command {
    char *current_command;  // readline()からの入力文字列
} t_command;

// env.h - 完全実装済み
typedef struct s_env {
    char *key;
    char *value; 
    struct s_env *next;
} t_env;
```

## 開発ガイドライン

### gitflow
- developブランチから新機能を開発
- 新機能はfeature/ブランチとして作成  
- mainブランチへの直接PRは禁止

### 設計ドキュメント連携
- `detailed_design.md`: 詳細なシステム設計仕様
- `function_specifications.md`: 関数レベルの仕様書
- `plan.md`: 実装計画（常に最新状態に更新）

### メモリリーク対策
- readlineライブラリ自体のメモリリークは許容
- 自作コードのメモリリークは完全に修正が必要
- valgrindで定期的に検査

## 開発時の注意点
- **42 Norm規約の厳守**: 関数は25行以内、1行80文字以内等
- **使用可能関数制限**: subject.mdに記載の関数のみ使用可能
- **シグナル処理制約**: グローバル変数は1つまで（`volatile sig_atomic_t`のみ）
- **メモリ管理**: libftのft_calloc/freeを使用、readlineの戻り値は必ずfree

## 重要な実装制約
- `t_command.current_command`は現在文字列のみ（将来的に構造化予定）
- 既存の`env()`シングルトンパターンを壊さないこと
- `exit_value()`システムとの整合性を保つこと

## 追加指示
- 日本語で返答してください
- gitflowを用いて開発を行なってください。新機能を追加する場合はdevelopブランチから派生させてfeatureブランチとしてください
- mainブランチにはPRを出さないでください
- 計画は常にplan.mdに出力して、plan.mdを書き換える様にしてください