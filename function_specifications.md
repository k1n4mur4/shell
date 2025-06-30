# Minishell 関数仕様書

## 1. レクサー（字句解析）関数仕様

### 1.1 lexer_create
```c
t_lexer *lexer_create(char *input);
```
**目的**: レクサーを初期化し、入力文字列を設定する  
**引数**: 
- `input` - 解析する入力文字列
**戻り値**: 初期化されたレクサー構造体のポインタ、失敗時はNULL  
**副作用**: メモリを動的に割り当てる  
**エラー条件**: 
- `input`がNULL
- メモリ割り当て失敗

### 1.2 lexer_tokenize
```c
t_token *lexer_tokenize(t_lexer *lexer);
```
**目的**: 入力文字列を完全にトークン化する  
**引数**: 
- `lexer` - 初期化済みのレクサー
**戻り値**: トークンリストの先頭ポインタ、失敗時はNULL  
**処理詳細**:
1. 文字列を1文字ずつ走査
2. 状態機械を使用してトークンを識別
3. 特殊文字（|, <, >, &&, ||）を認識
4. クォート処理を実行
5. 変数展開を実行

**状態遷移表**:
```
NORMAL → SQUOTE   (')
NORMAL → DQUOTE   (")
NORMAL → ESCAPE   (\)
NORMAL → VARIABLE ($)
SQUOTE → NORMAL   (')
DQUOTE → NORMAL   (")
DQUOTE → ESCAPE   (\)
DQUOTE → VARIABLE ($)
ESCAPE → NORMAL   (任意の文字)
```

### 1.3 lexer_expand_variables
```c
char *lexer_expand_variables(t_lexer *lexer, char *str);
```
**目的**: 文字列内の変数を展開する  
**引数**: 
- `lexer` - レクサーコンテキスト
- `str` - 展開する文字列
**戻り値**: 展開後の文字列、失敗時はNULL  
**処理詳細**:
1. `$VAR` または `${VAR}` 形式を検出
2. `$?` は直前の終了ステータスに展開
3. `$$` はプロセスIDに展開
4. 未定義変数は空文字列に展開
5. シングルクォート内では展開しない

## 2. パーサー（構文解析）関数仕様

### 2.1 parser_parse
```c
t_ast_node *parser_parse(t_parser *parser);
```
**目的**: トークンリストから抽象構文木を構築する  
**引数**: 
- `parser` - 初期化済みのパーサー
**戻り値**: AST のルートノード、エラー時はNULL  
**文法規則**:
```
command_list    : logical_or
                | command_list ';' logical_or

logical_or      : logical_and
                | logical_or OR_IF logical_and

logical_and     : pipeline
                | logical_and AND_IF pipeline

pipeline        : simple_command
                | pipeline '|' simple_command

simple_command  : cmd_word cmd_suffix
                | cmd_prefix cmd_word cmd_suffix
                | cmd_prefix
                | cmd_name cmd_suffix

cmd_suffix      : io_redirect
                | cmd_suffix io_redirect
                | WORD
                | cmd_suffix WORD

io_redirect     : io_file
                | IO_NUMBER io_file
                | io_here
                | IO_NUMBER io_here

io_file         : '<' filename
                | '>' filename
                | '>>' filename

io_here         : '<<' here_end

filename        : WORD
here_end        : WORD
```

### 2.2 parser_parse_simple_command
```c
t_simple_command *parser_parse_simple_command(t_parser *parser);
```
**目的**: 単純コマンドを解析する  
**引数**: 
- `parser` - パーサーコンテキスト
**戻り値**: 単純コマンド構造体、エラー時はNULL  
**処理詳細**:
1. コマンド名を取得
2. 引数を順次収集
3. リダイレクトを解析
4. `argv`配列を構築

## 3. 実行エンジン関数仕様

### 3.1 executor_execute_ast
```c
int executor_execute_ast(t_exec_context *ctx, t_ast_node *ast);
```
**目的**: AST を実行する  
**引数**: 
- `ctx` - 実行コンテキスト
- `ast` - 実行するAST
**戻り値**: 終了ステータス（0-255）  
**処理詳細**:
1. ASTのタイプに応じて適切な実行関数を呼び出す
2. エラー処理とクリーンアップを実行
3. 終了ステータスを設定

### 3.2 executor_execute_pipeline
```c
int executor_execute_pipeline(t_exec_context *ctx, t_pipeline *pipeline);
```
**目的**: パイプラインを実行する  
**引数**: 
- `ctx` - 実行コンテキスト
- `pipeline` - 実行するパイプライン
**戻り値**: 最後のコマンドの終了ステータス  
**アルゴリズム**:
```
1. コマンド数nに対してn-1個のパイプを作成
2. 各コマンドを子プロセスで実行:
   - 最初のコマンド: stdout をパイプに接続
   - 中間のコマンド: stdin をパイプから、stdout を次のパイプに接続
   - 最後のコマンド: stdin をパイプから接続
3. 全ての子プロセスを待機
4. 最後のプロセスの終了ステータスを返す
```

### 3.3 executor_setup_redirects
```c
int executor_setup_redirects(t_exec_context *ctx, t_redirect *redirects);
```
**目的**: リダイレクトを設定する  
**引数**: 
- `ctx` - 実行コンテキスト
- `redirects` - リダイレクトリスト
**戻り値**: 成功時0、失敗時-1  
**処理詳細**:
1. 元のファイルディスクリプタをバックアップ
2. リダイレクトタイプに応じてファイルを開く
3. `dup2()` でファイルディスクリプタを複製
4. ヒアドキュメントの場合は一時ファイルを作成

## 4. 組み込みコマンド関数仕様

### 4.1 builtin_echo
```c
int builtin_echo(t_exec_context *ctx, char **argv);
```
**目的**: echo コマンドを実装する  
**引数**: 
- `ctx` - 実行コンテキスト
- `argv` - コマンド引数配列
**戻り値**: 常に0  
**オプション**:
- `-n` - 最後の改行を出力しない
**処理詳細**:
1. `-n` オプションをチェック
2. 引数を空白区切りで出力
3. `-n` オプションがない場合は改行を出力

### 4.2 builtin_cd
```c
int builtin_cd(t_exec_context *ctx, char **argv);
```
**目的**: cd コマンドを実装する  
**引数**: 
- `ctx` - 実行コンテキスト
- `argv` - コマンド引数配列
**戻り値**: 成功時0、失敗時1  
**処理詳細**:
1. 引数なしの場合は `$HOME` に移動
2. `~` は `$HOME` に展開
3. `-` は前回のディレクトリ（`$OLDPWD`）に移動
4. `chdir()` を実行
5. `PWD` と `OLDPWD` 環境変数を更新

### 4.3 builtin_export
```c
int builtin_export(t_exec_context *ctx, char **argv);
```
**目的**: export コマンドを実装する  
**引数**: 
- `ctx` - 実行コンテキスト
- `argv` - コマンド引数配列
**戻り値**: 成功時0、失敗時1以上  
**処理詳細**:
1. 引数なしの場合は全てのexport済み変数を表示
2. `KEY=VALUE` 形式の場合は環境変数を設定
3. `KEY` のみの場合は既存変数をexport
4. 無効な変数名の場合はエラー

## 5. シグナル処理関数仕様

### 5.1 signal_handler_sigint
```c
void signal_handler_sigint(int sig);
```
**目的**: SIGINT (Ctrl+C) を処理する  
**引数**: 
- `sig` - シグナル番号
**処理詳細**:
1. グローバル変数に SIGINT 受信を記録
2. readline に割り込みを通知
3. 新しいプロンプトを表示

### 5.2 signals_setup_readline
```c
void signals_setup_readline(void);
```
**目的**: readline とシグナル処理を統合する  
**処理詳細**:
1. readline のシグナルハンドラを無効化
2. 独自のシグナルハンドラを設定
3. readline 関数の適切な呼び出し順序を確保

## 6. メモリ管理関数仕様

### 6.1 memory_pool_create
```c
t_memory_pool *memory_pool_create(size_t initial_capacity);
```
**目的**: メモリプールを作成する  
**引数**: 
- `initial_capacity` - 初期容量
**戻り値**: メモリプール構造体、失敗時はNULL  
**処理詳細**:
1. メモリプール構造体を割り当て
2. ポインタ配列を初期化
3. 容量とカウンタを設定

### 6.2 memory_pool_alloc
```c
void *memory_pool_alloc(t_memory_pool *pool, size_t size);
```
**目的**: メモリプールからメモリを割り当てる  
**引数**: 
- `pool` - メモリプール
- `size` - 割り当てサイズ
**戻り値**: 割り当てられたメモリ、失敗時はNULL  
**処理詳細**:
1. `malloc()` でメモリを割り当て
2. プールの配列に追加
3. 必要に応じて配列を拡張

## 7. エラーハンドリング関数仕様

### 7.1 error_set
```c
void error_set(t_error_code code, char *message);
```
**目的**: エラー情報を設定する  
**引数**: 
- `code` - エラーコード
- `message` - エラーメッセージ
**副作用**: グローバルエラー状態を更新

### 7.2 error_print
```c
void error_print(t_error_info *error);
```
**目的**: エラー情報を標準エラーに出力する  
**引数**: 
- `error` - エラー情報
**出力形式**: `minishell: {filename}:{line}:{column}: {message}`

## 8. パフォーマンス要件

### 8.1 時間計算量
- **レクサー**: O(n) - 入力文字列長に比例
- **パーサー**: O(n) - トークン数に比例
- **実行**: O(1) - 単純コマンドあたり

### 8.2 空間計算量
- **トークンリスト**: O(n) - 入力文字列長に比例
- **AST**: O(n) - トークン数に比例
- **メモリプール**: O(n) - 割り当て回数に比例

## 9. 安全性要件

### 9.1 メモリ安全性
- 全ての動的メモリ割り当てに対応する解放処理
- ダブルフリーの防止
- メモリリークの防止
- バッファオーバーフローの防止

### 9.2 シグナル安全性
- シグナルハンドラ内では安全な関数のみ使用
- 再入可能性の確保
- デッドロックの防止

## 10. テスト要件

### 10.1 単体テスト
各関数について以下の項目をテスト:
- 正常系の動作確認
- 異常系の動作確認（不正な引数等）
- 境界値の動作確認
- メモリリークの確認

### 10.2 統合テスト
- 実際のBashとの動作比較
- 複雑なコマンドライン入力の処理確認
- 長時間実行での安定性確認

この関数仕様に従って実装することで、堅牢で保守性の高いminishellが完成します。