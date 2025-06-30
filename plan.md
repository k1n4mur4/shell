# Minishell 実装計画

## 現在の状況分析

### 実装済みコンポーネント
- ✅ **基本インフラ**: Makefile、libft統合、OS依存処理
- ✅ **環境変数システム**: `env.c`完全実装（シングルトンパターン）
- ✅ **終了値管理**: `exit_value.c`完全実装
- ✅ **REPLループ**: `eval.c`の`reader_loop()`実装済み
- ✅ **readline統合**: 基本的な入力受付と履歴機能
- ✅ **メモリ管理**: `dispose_cmd.c`でコマンド構造体解放

### 未実装（スタブ状態）
- ❌ **パーサー**: `parser/parser.c`（空関数）
- ❌ **実行エンジン**: `execute/execute_cmd.c`（空関数）
- ❌ **シグナルハンドリング**: 完全未実装
- ❌ **組み込みコマンド**: 未実装
- ❌ **レクサー**: 未実装

## フェーズ1: 基盤システム強化

### 1.1 シグナルハンドリング実装
**目的**: Ctrl+C、Ctrl+\、子プロセス終了の適切な処理

**実装タスク**:
1. **グローバル変数定義**
   - `volatile sig_atomic_t g_signal_received`をmain.cに定義
   - シグナル状態を示すフラグ管理

2. **シグナルハンドラ実装**
   - `signal_handler_sigint()`: SIGINT (Ctrl+C)処理
   - `signal_handler_sigquit()`: SIGQUIT (Ctrl+\)処理 
   - `signal_handler_sigchld()`: 子プロセス終了処理

3. **readlineとの統合**
   - `rl_on_new_line()`、`rl_replace_line()`、`rl_redisplay()`使用
   - 割り込み時の適切なプロンプト再表示

**ファイル構成**:
```
includes/signals.h     # シグナル処理関数宣言
srcs/signals.c         # シグナルハンドラ実装
```

### 1.2 メモリ管理強化
**目的**: 安全なメモリ操作とリーク防止

**実装タスク**:
1. **安全なメモリ操作関数**
   - `safe_malloc()`, `safe_free()`, `safe_strdup()`
   - NULLチェックとエラーハンドリング統合

2. **メモリプール（オプション）**
   - 頻繁な割り当て/解放の効率化
   - 一括解放機能

## フェーズ2: 基本コマンド実行

### 2.1 単純コマンド実行機能
**目的**: `t_command.current_command`文字列の基本実行

**実装タスク**:
1. **execute_cmd.c拡張**
   ```c
   void execute_command(t_command *command)
   {
       // 1. 基本的な文字列解析（空白区切り）
       // 2. 組み込みコマンド判定
       // 3. 外部コマンド実行
       // 4. 終了値設定
   }
   ```

2. **コマンド種別判定**
   - 組み込みコマンド vs 外部コマンド
   - 空コマンド・コメント処理

3. **基本的なプロセス管理**
   - `fork()` + `execve()` + `waitpid()`
   - 終了ステータスの`exit_value()`連携

### 2.2 PATH環境変数対応
**目的**: コマンド検索機能の実装

**実装タスク**:
1. **コマンド検索関数**
   ```c
   char *find_command_path(char *command);
   ```
   - PATH環境変数の解析
   - 各ディレクトリでの実行ファイル検索
   - `access()`での実行権限確認

2. **既存env()システムとの統合**
   - `env(NULL, GET)`でPATH取得
   - 環境変数更新時の自動対応

## フェーズ3: 組み込みコマンド実装

### 3.1 必須組み込みコマンド
**実装順序と理由**:

1. **exit** (最優先)
   - 既存の`exit_value()`システム活用
   - 引数解析とエラーハンドリング

2. **env** 
   - 既存の`env(NULL, SHOW)`活用
   - 最も実装が簡単

3. **pwd**
   - `getcwd()`使用
   - 単純な出力のみ

4. **cd**
   - `chdir()`使用
   - PWD、OLDPWD環境変数更新
   - 既存env()システムとの連携

5. **echo**
   - `-n`オプション処理
   - 引数の適切な出力

6. **export/unset**
   - 環境変数操作
   - 既存env()システムの拡張

**ファイル構成**:
```
includes/builtins.h           # 組み込みコマンド宣言
srcs/builtins/builtin_exit.c
srcs/builtins/builtin_env.c
srcs/builtins/builtin_pwd.c
srcs/builtins/builtin_cd.c
srcs/builtins/builtin_echo.c
srcs/builtins/builtin_export.c
srcs/builtins/builtin_unset.c
```

## フェーズ4: レクサー・パーサー基盤

### 4.1 基本字句解析
**目的**: 文字列を意味のあるトークンに分割

**実装タスク**:
1. **トークン構造定義**
   ```c
   typedef enum {
       TOKEN_WORD, TOKEN_PIPE, TOKEN_REDIRECT_IN,
       TOKEN_REDIRECT_OUT, TOKEN_APPEND, TOKEN_HEREDOC
   } t_token_type;
   
   typedef struct s_token {
       t_token_type type;
       char *value;
       struct s_token *next;
   } t_token;
   ```

2. **基本的なトークン化**
   - 空白区切りでの単語分割
   - 特殊文字（|, <, >, <<, >>）の認識
   - クォート処理の基盤

### 4.2 構文解析基盤
**目的**: トークンから実行可能な構造を構築

**実装タスク**:
1. **コマンド構造の拡張**
   ```c
   typedef struct s_simple_command {
       char **argv;
       int argc;
       // 将来: リダイレクト情報
   } t_simple_command;
   ```

2. **基本的な構文解析**
   - 単純コマンド解析
   - エラーハンドリング

## フェーズ5: 高度な機能（将来実装）

### 5.1 パイプライン
- 複数コマンドの連結
- ファイルディスクリプタ管理

### 5.2 リダイレクト
- 入出力リダイレクト
- ヒアドキュメント

### 5.3 変数展開
- `$VAR`, `$?` 処理
- クォート内での展開制御

## 実装戦略

### 段階的アプローチ
1. **既存システムを壊さない**: `env()`, `exit_value()`の維持
2. **最小実装から開始**: 単純なケースから複雑なケースへ
3. **テスト駆動**: 各フェーズでvalgrind検証

### リスク管理
1. **メモリリーク**: 各フェーズでvalgrind検証
2. **Norm規約**: 関数サイズ・行数制限の遵守
3. **互換性**: 既存のlibftシステムとの整合性

### 完了基準
- [ ] フェーズ1: シグナル処理でCtrl+Cが適切に動作
- [ ] フェーズ2: 基本的な外部コマンド実行（`/bin/ls`等）
- [ ] フェーズ3: 全組み込みコマンドが動作
- [ ] フェーズ4: 基本的なパイプラインが動作
- [ ] 全体: メモリリークなし、Norm規約準拠

## 次のアクション
フェーズ1のシグナルハンドリング実装から開始