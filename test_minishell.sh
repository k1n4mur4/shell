#!/bin/bash

# Minishell自動テストスクリプト
# 使用方法: ./test_minishell.sh

# 色定義
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

# テスト結果カウンタ
PASSED=0
FAILED=0

# minishellのパス
MINISHELL="./minishell"

# テスト用一時ファイル
TEST_DIR="/tmp/minishell_test_$$"
mkdir -p "$TEST_DIR"

# 終了時のクリーンアップ
cleanup() {
    rm -rf "$TEST_DIR"
}
trap cleanup EXIT

# テスト関数
run_test() {
    local test_name="$1"
    local command="$2"
    local expected="$3"
    
    echo -n "Testing: $test_name ... "
    
    # minishellでコマンドを実行
    echo "$command" | $MINISHELL 2>/dev/null | grep -v "minishell>" > "$TEST_DIR/minishell_output"
    local minishell_exit=$?
    
    # bashでコマンドを実行（期待値として使用）
    if [ -n "$expected" ]; then
        echo "$expected" > "$TEST_DIR/expected_output"
    else
        echo "$command" | bash 2>/dev/null > "$TEST_DIR/expected_output"
    fi
    
    # 結果を比較
    if diff -q "$TEST_DIR/minishell_output" "$TEST_DIR/expected_output" > /dev/null; then
        echo -e "${GREEN}PASSED${NC}"
        ((PASSED++))
    else
        echo -e "${RED}FAILED${NC}"
        echo "  Command: $command"
        echo "  Expected:"
        cat "$TEST_DIR/expected_output" | sed 's/^/    /'
        echo "  Got:"
        cat "$TEST_DIR/minishell_output" | sed 's/^/    /'
        ((FAILED++))
    fi
}

# ビルトインコマンドのテスト（特殊な比較が必要）
test_builtin() {
    local test_name="$1"
    local command="$2"
    
    echo -n "Testing builtin: $test_name ... "
    
    # テストを実行（結果は手動で確認が必要）
    echo "$command" | timeout 2 $MINISHELL > /dev/null 2>&1
    local exit_code=$?
    
    if [ $exit_code -eq 124 ]; then
        echo -e "${RED}TIMEOUT${NC}"
        ((FAILED++))
    else
        echo -e "${YELLOW}MANUAL CHECK REQUIRED${NC}"
    fi
}

echo "===== Minishell Automated Test Suite ====="
echo

# ビルド確認
if [ ! -f "$MINISHELL" ]; then
    echo -e "${RED}Error: minishell not found. Please run 'make' first.${NC}"
    exit 1
fi

# 1. 基本的なコマンド実行
echo "=== Basic Command Execution ==="
run_test "echo simple" "echo Hello World" "Hello World"
run_test "echo with quotes" "echo \"Hello World\"" "Hello World"
run_test "ls command" "/bin/ls /dev/null" "/dev/null"
run_test "true command" "/usr/bin/true && echo OK" "OK"
run_test "false command" "/usr/bin/false || echo OK" "OK"

# 2. パイプライン
echo -e "\n=== Pipeline Tests ==="
run_test "simple pipe" "echo Hello | cat" "Hello"
run_test "multiple pipes" "echo Hello | cat | cat" "Hello"
run_test "pipe with grep" "echo -e 'line1\nline2\nline3' | grep line2" "line2"

# 3. リダイレクション
echo -e "\n=== Redirection Tests ==="
# 出力リダイレクション
echo "test content" > "$TEST_DIR/input.txt"
run_test "input redirection" "cat < $TEST_DIR/input.txt" "test content"

# ヒアドキュメント（簡易テスト）
test_builtin "heredoc" "cat << EOF
Hello
World
EOF"

# 4. 環境変数
echo -e "\n=== Environment Variable Tests ==="
run_test "HOME variable" "echo \$HOME" "$HOME"
run_test "PATH variable" "echo \$PATH | grep -q '/usr/bin' && echo OK" "OK"

# 5. クォート処理
echo -e "\n=== Quote Processing ==="
run_test "single quotes" "echo 'Hello \$HOME'" "Hello \$HOME"
run_test "double quotes with var" "echo \"Home is \$HOME\"" "Home is $HOME"

# 6. エラーケース
echo -e "\n=== Error Cases ==="
test_builtin "command not found" "nonexistentcommand"
test_builtin "permission denied" "/etc/passwd"

# 7. ビルトインコマンド（対話的テストが必要）
echo -e "\n=== Builtin Commands (Manual verification needed) ==="
test_builtin "pwd" "pwd"
test_builtin "env" "env | grep -q PATH"
test_builtin "export" "export TEST_VAR=hello && echo \$TEST_VAR"

# 結果サマリー
echo
echo "===== Test Summary ====="
echo -e "Passed: ${GREEN}$PASSED${NC}"
echo -e "Failed: ${RED}$FAILED${NC}"
echo -e "Total: $((PASSED + FAILED))"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}All automated tests passed!${NC}"
    exit 0
else
    echo -e "\n${RED}Some tests failed. Please check the output above.${NC}"
    exit 1
fi