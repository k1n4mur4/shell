# Minishell 詳細設計書

## 1. 全体アーキテクチャ

### 1.1 モジュール構成図
```
minishell
├── Core System
│   ├── Shell Manager (shell.c)
│   ├── Signal Handler (signals/)
│   └── Memory Manager (memory/)
├── Input Processing
│   ├── Lexer (lexer/)
│   ├── Parser (parser/)
│   └── Readline Interface (rl.c)
├── Execution Engine
│   ├── Command Executor (executor/)
│   ├── Builtin Commands (builtins/)
│   └── Process Manager (process/)
├── Environment System
│   ├── Environment Variables (env.c)
│   └── Exit Status Manager (exit_value.c)
└── Utilities
    ├── libft Integration
    └── Error Handling
```

### 1.2 データフロー
```
Input → Lexer → Parser → Executor → Output
  ↓       ↓       ↓        ↓        ↓
Signal  Token   AST   Commands  Status
Handler  List   Tree  Execution Values
```

## 2. 現在のコードベース分析

### 2.1 既存のデータ構造
```c
// env.h - 環境変数管理（実装済み）
typedef struct s_env {
    char        *key;
    char        *value;
    struct s_env *next;
} t_env;

// command.h - コマンド表現（基本構造のみ）
typedef struct s_word_desc {
    char    *word;
    int     flags;
} t_word_desc;

typedef struct s_word_list {
    struct s_word_list  *next;
    t_word_desc         *word;
} t_word_list;

typedef struct s_command {
    char    *current_command;  // 現在は単純な文字列のみ
} t_command;
```

### 2.2 既存の列挙型
```c
// define.h
typedef enum e_type {
    SET,    // 環境変数設定
    GET,    // 環境変数取得
    SHOW,   // 環境変数表示
    ADD,    // 環境変数追加
    FREE,   // メモリ解放
    RESET   // リセット
} t_type;
```

## 3. 詳細設計

### 3.1 新しいデータ構造設計

#### 3.1.1 トークン系（lexer/token.h）
```c
// トークンの種類
typedef enum e_token_type {
    TOKEN_WORD,             // 通常の単語
    TOKEN_PIPE,             // |
    TOKEN_REDIRECT_IN,      // <
    TOKEN_REDIRECT_OUT,     // >
    TOKEN_APPEND,           // >>
    TOKEN_HEREDOC,          // <<
    TOKEN_AND_IF,           // &&
    TOKEN_OR_IF,            // ||
    TOKEN_LPAREN,           // (
    TOKEN_RPAREN,           // )
    TOKEN_SEMICOLON,        // ;
    TOKEN_NEWLINE,          // \n
    TOKEN_EOF,              // 入力終了
    TOKEN_ERROR             // エラー
} t_token_type;

// トークン構造体
typedef struct s_token {
    t_token_type    type;
    char            *value;
    int             line;       // 行番号（エラー表示用）
    int             column;     // 列番号（エラー表示用）
    struct s_token  *next;
} t_token;

// レクサーの状態
typedef enum e_lexer_state {
    LEXER_STATE_NORMAL,
    LEXER_STATE_SQUOTE,     // シングルクォート内
    LEXER_STATE_DQUOTE,     // ダブルクォート内
    LEXER_STATE_ESCAPE,     // エスケープ文字後
    LEXER_STATE_VARIABLE    // 変数展開中
} t_lexer_state;

// レクサーコンテキスト
typedef struct s_lexer {
    char            *input;
    int             pos;
    int             line;
    int             column;
    t_lexer_state   state;
    t_token         *tokens;
    t_token         *current;
} t_lexer;
```

#### 3.1.2 リダイレクト系（parser/redirect.h）
```c
// リダイレクトの種類
typedef enum e_redirect_type {
    REDIRECT_INPUT,         // < file
    REDIRECT_OUTPUT,        // > file
    REDIRECT_APPEND,        // >> file
    REDIRECT_HEREDOC,       // << delimiter
    REDIRECT_INPUT_DUP,     // <&n
    REDIRECT_OUTPUT_DUP     // >&n
} t_redirect_type;

// リダイレクト構造体
typedef struct s_redirect {
    t_redirect_type     type;
    int                 fd_from;    // リダイレクト元のfd
    int                 fd_to;      // リダイレクト先のfd
    char                *filename;  // ファイル名
    char                *delimiter; // ヒアドキュメントの区切り文字
    struct s_redirect   *next;
} t_redirect;
```

#### 3.1.3 コマンド系（parser/command.h）
```c
// 既存のt_commandを拡張
typedef struct s_simple_command {
    char                        **argv;         // コマンドと引数
    int                         argc;           // 引数の数
    t_redirect                  *redirects;     // リダイレクトリスト
    struct s_simple_command     *next;          // パイプライン用
} t_simple_command;

// パイプライン
typedef struct s_pipeline {
    t_simple_command    *commands;      // コマンドリスト
    int                 command_count;  // コマンド数
    int                 background;     // バックグラウンド実行
} t_pipeline;

// 論理演算子
typedef enum e_logical_op {
    LOGICAL_NONE,
    LOGICAL_AND,    // &&
    LOGICAL_OR      // ||
} t_logical_op;

// 複合コマンド
typedef struct s_compound_command {
    t_pipeline                  *pipeline;
    t_logical_op                logical_op;
    struct s_compound_command   *next;
} t_compound_command;

// AST（抽象構文木）のノード
typedef enum e_ast_type {
    AST_SIMPLE_COMMAND,
    AST_PIPELINE,
    AST_LOGICAL_AND,
    AST_LOGICAL_OR,
    AST_SUBSHELL,
    AST_COMMAND_LIST
} t_ast_type;

typedef struct s_ast_node {
    t_ast_type              type;
    union {
        t_simple_command    *simple_cmd;
        t_pipeline          *pipeline;
        struct {
            struct s_ast_node   *left;
            struct s_ast_node   *right;
        } binary;
        struct s_ast_node   *child;
    } data;
    struct s_ast_node       *next;
} t_ast_node;
```

#### 3.1.4 実行系（executor/executor.h）
```c
// 実行コンテキスト
typedef struct s_exec_context {
    t_env       *env;           // 環境変数
    int         exit_status;    // 終了ステータス
    int         stdin_fd;       // 標準入力fd
    int         stdout_fd;      // 標準出力fd
    int         stderr_fd;      // 標準エラーfd
    pid_t       *pids;          // 子プロセスのPID配列
    int         pid_count;      // 子プロセス数
    int         interactive;    // 対話モードかどうか
} t_exec_context;

// プロセスステータス
typedef struct s_process_status {
    pid_t       pid;
    int         status;
    int         exit_code;
    int         signaled;
    int         signal_num;
} t_process_status;
```

#### 3.1.5 シグナル系（signals/signals.h）
```c
// シグナルの種類
typedef enum e_signal_type {
    SIGNAL_NONE,
    SIGNAL_INT,     // SIGINT (Ctrl+C)
    SIGNAL_QUIT,    // SIGQUIT (Ctrl+\)
    SIGNAL_TERM,    // SIGTERM
    SIGNAL_CHLD     // SIGCHLD
} t_signal_type;

// シグナルハンドラの情報
typedef struct s_signal_info {
    t_signal_type   type;
    int             received;
    pid_t           sender_pid;
    int             exit_status;
} t_signal_info;

// グローバル変数（1つのみ許可）
extern volatile sig_atomic_t g_signal_received;
```

#### 3.1.6 シェル管理系（core/shell.h）
```c
// シェルの状態
typedef enum e_shell_state {
    SHELL_NORMAL,
    SHELL_HEREDOC,
    SHELL_SUBSHELL,
    SHELL_EXITING
} t_shell_state;

// シェルコンテキスト
typedef struct s_shell_context {
    t_env           *env;           // 環境変数
    t_exec_context  *exec_ctx;      // 実行コンテキスト
    t_shell_state   state;          // シェルの状態
    int             exit_status;    // 終了ステータス
    int             running;        // 実行中フラグ
    char            *prompt;        // プロンプト文字列
    char            *cwd;           // 現在のディレクトリ
    int             interactive;    // 対話モード
    int             login_shell;    // ログインシェル
} t_shell_context;
```

### 3.2 関数インターフェース設計

#### 3.2.1 レクサー関数（lexer/lexer.h）
```c
// 初期化・解放
t_lexer         *lexer_create(char *input);
void            lexer_destroy(t_lexer *lexer);

// トークン化
t_token         *lexer_tokenize(t_lexer *lexer);
t_token         *lexer_next_token(t_lexer *lexer);

// トークン操作
t_token         *token_create(t_token_type type, char *value);
void            token_destroy(t_token *token);
void            token_list_destroy(t_token *tokens);
char            *token_type_to_string(t_token_type type);

// 文字列処理
char            *lexer_expand_variables(t_lexer *lexer, char *str);
char            *lexer_handle_quotes(t_lexer *lexer, char *str);
int             lexer_is_special_char(char c);
```

#### 3.2.2 パーサー関数（parser/parser.h）
```c
// パーサー初期化・解放
typedef struct s_parser {
    t_token     *tokens;
    t_token     *current;
    int         error_count;
    char        *error_message;
} t_parser;

t_parser        *parser_create(t_token *tokens);
void            parser_destroy(t_parser *parser);

// 構文解析
t_ast_node      *parser_parse(t_parser *parser);
t_ast_node      *parser_parse_command_list(t_parser *parser);
t_ast_node      *parser_parse_logical_or(t_parser *parser);
t_ast_node      *parser_parse_logical_and(t_parser *parser);
t_ast_node      *parser_parse_pipeline(t_parser *parser);
t_simple_command *parser_parse_simple_command(t_parser *parser);
t_redirect      *parser_parse_redirects(t_parser *parser);

// AST操作
t_ast_node      *ast_create_node(t_ast_type type);
void            ast_destroy(t_ast_node *node);
void            ast_print(t_ast_node *node, int indent);

// エラーハンドリング
void            parser_error(t_parser *parser, char *message);
int             parser_expect_token(t_parser *parser, t_token_type type);
int             parser_consume_token(t_parser *parser, t_token_type type);
```

#### 3.2.3 実行系関数（executor/executor.h）
```c
// 実行コンテキスト管理
t_exec_context  *exec_context_create(t_env *env);
void            exec_context_destroy(t_exec_context *ctx);

// コマンド実行
int             executor_execute_ast(t_exec_context *ctx, t_ast_node *ast);
int             executor_execute_pipeline(t_exec_context *ctx, t_pipeline *pipeline);
int             executor_execute_simple_command(t_exec_context *ctx, t_simple_command *cmd);

// プロセス管理
pid_t           executor_fork_and_exec(t_exec_context *ctx, t_simple_command *cmd);
int             executor_wait_for_processes(t_exec_context *ctx);
void            executor_setup_pipes(t_exec_context *ctx, t_pipeline *pipeline);

// ファイルディスクリプタ管理
int             executor_setup_redirects(t_exec_context *ctx, t_redirect *redirects);
void            executor_restore_fds(t_exec_context *ctx);
int             executor_open_file(char *filename, t_redirect_type type);

// コマンド検索
char            *executor_find_command(t_exec_context *ctx, char *command);
int             executor_is_builtin(char *command);
```

#### 3.2.4 組み込みコマンド関数（builtins/builtins.h）
```c
// 組み込みコマンド関数ポインタ
typedef int (*t_builtin_func)(t_exec_context *ctx, char **argv);

// 組み込みコマンド構造体
typedef struct s_builtin {
    char            *name;
    t_builtin_func  func;
    char            *description;
} t_builtin;

// 各組み込みコマンド
int builtin_echo(t_exec_context *ctx, char **argv);
int builtin_cd(t_exec_context *ctx, char **argv);
int builtin_pwd(t_exec_context *ctx, char **argv);
int builtin_export(t_exec_context *ctx, char **argv);
int builtin_unset(t_exec_context *ctx, char **argv);
int builtin_env(t_exec_context *ctx, char **argv);
int builtin_exit(t_exec_context *ctx, char **argv);

// 組み込みコマンド管理
t_builtin       *builtin_find(char *name);
int             builtin_execute(t_exec_context *ctx, char *name, char **argv);
```

#### 3.2.5 シグナル処理関数（signals/signals.h）
```c
// シグナルハンドラ設定
void            signals_setup_handlers(void);
void            signals_reset_handlers(void);
void            signals_block_all(void);
void            signals_unblock_all(void);

// 各シグナルハンドラ
void            signal_handler_sigint(int sig);
void            signal_handler_sigquit(int sig);
void            signal_handler_sigchld(int sig);

// シグナル状態管理
t_signal_info   *signals_get_info(void);
void            signals_clear_info(void);
int             signals_was_interrupted(void);

// readlineとの統合
void            signals_setup_readline(void);
void            signals_handle_readline_interrupt(void);
```

### 3.3 メモリ管理設計

#### 3.3.1 メモリ管理戦略
```c
// memory/memory.h
typedef struct s_memory_pool {
    void            **ptrs;
    size_t          capacity;
    size_t          count;
} t_memory_pool;

// メモリプール管理
t_memory_pool   *memory_pool_create(size_t initial_capacity);
void            memory_pool_destroy(t_memory_pool *pool);
void            *memory_pool_alloc(t_memory_pool *pool, size_t size);
void            memory_pool_free_all(t_memory_pool *pool);

// 安全なメモリ操作
void            *safe_malloc(size_t size);
void            *safe_calloc(size_t count, size_t size);
char            *safe_strdup(const char *str);
void            safe_free(void **ptr);
```

### 3.4 エラーハンドリング設計

#### 3.4.1 エラーコード定義
```c
// error/error.h
typedef enum e_error_code {
    ERROR_NONE = 0,
    ERROR_SYNTAX = 2,
    ERROR_COMMAND_NOT_FOUND = 127,
    ERROR_PERMISSION_DENIED = 126,
    ERROR_OUT_OF_MEMORY = 128,
    ERROR_INVALID_ARGUMENT = 129,
    ERROR_IO_ERROR = 130,
    ERROR_SIGNAL_RECEIVED = 131
} t_error_code;

// エラー情報
typedef struct s_error_info {
    t_error_code    code;
    char            *message;
    char            *filename;
    int             line;
    int             column;
} t_error_info;

// エラーハンドリング関数
void            error_set(t_error_code code, char *message);
void            error_set_with_location(t_error_code code, char *message, 
                                      char *filename, int line, int column);
t_error_info    *error_get_last(void);
void            error_clear(void);
void            error_print(t_error_info *error);
```

## 4. ファイル構成設計

### 4.1 新しいディレクトリ構造
```
shell/
├── includes/
│   ├── core/
│   │   ├── shell.h
│   │   └── memory.h
│   ├── lexer/
│   │   ├── lexer.h
│   │   └── token.h
│   ├── parser/
│   │   ├── parser.h
│   │   ├── ast.h
│   │   └── redirect.h
│   ├── executor/
│   │   ├── executor.h
│   │   └── process.h
│   ├── builtins/
│   │   └── builtins.h
│   ├── signals/
│   │   └── signals.h
│   └── error/
│       └── error.h
├── srcs/
│   ├── core/
│   │   ├── shell_context.c
│   │   ├── shell_init.c
│   │   └── memory_pool.c
│   ├── lexer/
│   │   ├── lexer.c
│   │   ├── tokenizer.c
│   │   ├── quote_handler.c
│   │   └── variable_expansion.c
│   ├── parser/
│   │   ├── parser.c
│   │   ├── ast_builder.c
│   │   ├── redirect_parser.c
│   │   └── command_parser.c
│   ├── executor/
│   │   ├── executor.c
│   │   ├── command_executor.c
│   │   ├── pipeline_executor.c
│   │   ├── redirect_handler.c
│   │   └── process_manager.c
│   ├── builtins/
│   │   ├── builtin_manager.c
│   │   ├── builtin_echo.c
│   │   ├── builtin_cd.c
│   │   ├── builtin_pwd.c
│   │   ├── builtin_export.c
│   │   ├── builtin_unset.c
│   │   ├── builtin_env.c
│   │   └── builtin_exit.c
│   ├── signals/
│   │   ├── signal_handler.c
│   │   ├── signal_manager.c
│   │   └── readline_integration.c
│   └── error/
│       ├── error_handler.c
│       └── error_messages.c
```

### 4.2 既存ファイルとの統合
- 既存の `env.c`, `exit_value.c`, `eval.c` は保持
- 新しいシステムとの橋渡し役として機能
- 段階的な移行を可能にする

## 5. 実装の優先順位

### 5.1 フェーズ1: 基盤整備
1. **シグナル処理の実装**
   - `signals/` ディレクトリ作成
   - 基本的なシグナルハンドラ実装
   - readlineとの統合

2. **メモリ管理の改善**
   - メモリプールの実装
   - 安全なメモリ操作関数

3. **エラーハンドリングの統一**
   - エラーコード体系の確立
   - エラー情報の構造化

### 5.2 フェーズ2: レクサー実装
1. **基本的なトークン化**
   - `lexer/` ディレクトリ作成
   - トークン構造体の実装
   - 基本的な字句解析

2. **クォート処理**
   - シングル・ダブルクォート処理
   - エスケープシーケンス処理

3. **変数展開**
   - `$VAR` 形式の処理
   - `$?` 終了ステータス処理

### 5.3 フェーズ3以降
- パーサー、実行エンジン、組み込みコマンドの順次実装

## 6. テスト設計

### 6.1 単体テスト
```c
// test/test_lexer.c
void test_lexer_basic_tokenization(void);
void test_lexer_quote_handling(void);
void test_lexer_variable_expansion(void);

// test/test_parser.c
void test_parser_simple_command(void);
void test_parser_pipeline(void);
void test_parser_redirects(void);
```

### 6.2 統合テスト
```bash
# test/integration_tests.sh
test_basic_commands()
test_pipeline_execution()
test_redirect_functionality()
test_signal_handling()
```

この詳細設計により、現在のコードを変更することなく、段階的に機能を追加できる拡張可能な設計となっています。