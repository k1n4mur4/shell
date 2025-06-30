#!/bin/bash

# Source the main test script functions
source ./test_lexer.sh 2>/dev/null || {
    echo "Error: test_lexer.sh not found. Please run this script from the minishell directory."
    exit 1
}

# Reset counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

echo "========================================="
echo "    MINISHELL LEXER EXTENDED TESTS"
echo "========================================="
echo ""

# Additional test cases
echo "=== Special Characters ==="
run_test "Semicolon" \
    "echo hello; echo world" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: 'hello' (flags: 0x0)
Word[2]: ';' (flags: 0x0)
Word[3]: 'echo' (flags: 0x0)
Word[4]: 'world' (flags: 0x0)"

run_test "Parentheses" \
    "(echo hello)" \
    "Word[0]: '(echo' (flags: 0x0)
Word[1]: 'hello)' (flags: 0x0)"

run_test "Backslash" \
    "echo \\ test" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: '\\' (flags: 0x0)
Word[2]: 'test' (flags: 0x0)"

run_test "Ampersand" \
    "sleep 5 &" \
    "Word[0]: 'sleep' (flags: 0x0)
Word[1]: '5' (flags: 0x0)
Word[2]: '&' (flags: 0x0)"

echo ""
echo "=== Unicode and Special Characters ==="
run_test "Unicode characters" \
    "echo 'こんにちは'" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: ''こんにちは'' (flags: 0x2)"

run_test "Emoji" \
    "echo '🚀 Hello'" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: ''🚀 Hello'' (flags: 0x2)"

echo ""
echo "=== Nested Quotes Variations ==="
run_test "Quote inside different quote type" \
    "echo \"hello 'world'\"" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: '\"hello 'world'\"' (flags: 0x2)"

run_test "Double quote inside single quote" \
    "echo 'hello \"world\"'" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: ''hello \"world\"'' (flags: 0x2)"

echo ""
echo "=== Extreme Cases ==="
run_test "Very long word" \
    "echo abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789' (flags: 0x0)"

run_test "Many arguments" \
    "echo a b c d e f g h i j" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: 'a' (flags: 0x0)
Word[2]: 'b' (flags: 0x0)
Word[3]: 'c' (flags: 0x0)
Word[4]: 'd' (flags: 0x0)
Word[5]: 'e' (flags: 0x0)
Word[6]: 'f' (flags: 0x0)
Word[7]: 'g' (flags: 0x0)
Word[8]: 'h' (flags: 0x0)
Word[9]: 'i' (flags: 0x0)
Word[10]: 'j' (flags: 0x0)"

run_test "All operators combined" \
    "< in > out >> app << EOF | cmd && next" \
    "Word[0]: '<' (flags: 0x0)
Word[1]: 'in' (flags: 0x0)
Word[2]: '>' (flags: 0x0)
Word[3]: 'out' (flags: 0x0)
Word[4]: '>>' (flags: 0x0)
Word[5]: 'app' (flags: 0x0)
Word[6]: '<<' (flags: 0x0)
Word[7]: 'EOF' (flags: 0x0)
Word[8]: '|' (flags: 0x4)
Word[9]: 'cmd' (flags: 0x0)
Word[10]: '&&' (flags: 0x0)
Word[11]: 'next' (flags: 0x0)"

echo ""
echo "=== Real World Examples ==="
run_test "Git command" \
    "git commit -m \"Initial commit\"" \
    "Word[0]: 'git' (flags: 0x0)
Word[1]: 'commit' (flags: 0x0)
Word[2]: '-m' (flags: 0x0)
Word[3]: '\"Initial commit\"' (flags: 0x2)"

run_test "Find command" \
    "find . -name '*.txt' -type f" \
    "Word[0]: 'find' (flags: 0x0)
Word[1]: '.' (flags: 0x0)
Word[2]: '-name' (flags: 0x0)
Word[3]: ''*.txt'' (flags: 0x2)
Word[4]: '-type' (flags: 0x0)
Word[5]: 'f' (flags: 0x0)"

run_test "Grep with regex" \
    "grep -E '^[0-9]+\$' file.txt" \
    "Word[0]: 'grep' (flags: 0x0)
Word[1]: '-E' (flags: 0x0)
Word[2]: ''^[0-9]+\$'' (flags: 0x3)
Word[3]: 'file.txt' (flags: 0x0)"

run_test "Complex pipe with sed" \
    "cat file | sed 's/old/new/g' | sort" \
    "Word[0]: 'cat' (flags: 0x0)
Word[1]: 'file' (flags: 0x0)
Word[2]: '|' (flags: 0x4)
Word[3]: 'sed' (flags: 0x0)
Word[4]: ''s/old/new/g'' (flags: 0x2)
Word[5]: '|' (flags: 0x4)
Word[6]: 'sort' (flags: 0x0)"

echo ""
echo "=== Whitespace Variations ==="
run_test "Leading spaces" \
    "   echo hello" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: 'hello' (flags: 0x0)"

run_test "Trailing spaces" \
    "echo hello   " \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: 'hello' (flags: 0x0)"

run_test "Mixed whitespace" \
    " 	echo 	 hello 	" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: 'hello' (flags: 0x0)"

echo ""
echo "========================================="
echo "        EXTENDED TEST SUMMARY"
echo "========================================="
echo "Total tests: $TOTAL_TESTS"
echo -e "Passed: ${GREEN}$PASSED_TESTS${NC}"
echo -e "Failed: ${RED}$FAILED_TESTS${NC}"

if [[ $FAILED_TESTS -eq 0 ]]; then
    echo -e "\n${GREEN}All extended tests passed!${NC}"
    exit 0
else
    echo -e "\n${RED}Some extended tests failed.${NC}"
    exit 1
fi