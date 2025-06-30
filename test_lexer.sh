#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test configuration
MINISHELL="./minishell"
ERRORS_ONLY=0
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -e|--errors-only)
            ERRORS_ONLY=1
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [options]"
            echo "Options:"
            echo "  -e, --errors-only    Show only failed tests"
            echo "  -h, --help          Show this help message"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Test function
run_test() {
    local description="$1"
    local input="$2"
    local expected="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Run the test
    output=$(echo "$input" | $MINISHELL 2>&1 | grep -A100 "Parsed command line:" | grep "Word\[" | sed 's/minishell\$//')
    
    # Compare output
    if [[ "$output" == "$expected" ]]; then
        PASSED_TESTS=$((PASSED_TESTS + 1))
        if [[ $ERRORS_ONLY -eq 0 ]]; then
            echo -e "${GREEN}✓${NC} $description"
        fi
    else
        FAILED_TESTS=$((FAILED_TESTS + 1))
        echo -e "${RED}✗${NC} $description"
        echo "  Input: '$input'"
        echo "  Expected:"
        echo "$expected" | sed 's/^/    /'
        echo "  Got:"
        echo "$output" | sed 's/^/    /'
        echo ""
    fi
}

# Test for syntax errors (should return no output)
run_error_test() {
    local description="$1"
    local input="$2"
    local expected_error="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Run the test
    error_output=$(echo "$input" | $MINISHELL 2>&1 | grep "minishell:" | head -1)
    word_output=$(echo "$input" | $MINISHELL 2>&1 | grep "Word\[")
    
    # Check if error message appears and no words are parsed
    if [[ "$error_output" == *"$expected_error"* ]] && [[ -z "$word_output" ]]; then
        PASSED_TESTS=$((PASSED_TESTS + 1))
        if [[ $ERRORS_ONLY -eq 0 ]]; then
            echo -e "${GREEN}✓${NC} $description"
        fi
    else
        FAILED_TESTS=$((FAILED_TESTS + 1))
        echo -e "${RED}✗${NC} $description"
        echo "  Input: '$input'"
        echo "  Expected error containing: '$expected_error'"
        echo "  Got error: '$error_output'"
        if [[ -n "$word_output" ]]; then
            echo "  Unexpected word output:"
            echo "$word_output" | sed 's/^/    /'
        fi
        echo ""
    fi
}

echo "========================================="
echo "        MINISHELL LEXER TESTS"
echo "========================================="
echo ""

# Basic word splitting tests
echo "=== Basic Word Splitting ==="
run_test "Simple command" \
    "ls" \
    "Word[0]: 'ls' (flags: 0x0)"

run_test "Command with argument" \
    "ls -la" \
    "Word[0]: 'ls' (flags: 0x0)
Word[1]: '-la' (flags: 0x0)"

run_test "Multiple arguments" \
    "echo hello world" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: 'hello' (flags: 0x0)
Word[2]: 'world' (flags: 0x0)"

run_test "Multiple spaces" \
    "ls    -la     file" \
    "Word[0]: 'ls' (flags: 0x0)
Word[1]: '-la' (flags: 0x0)
Word[2]: 'file' (flags: 0x0)"

run_test "Tabs as separators" \
    "ls	-la	file" \
    "Word[0]: 'ls' (flags: 0x0)
Word[1]: '-la' (flags: 0x0)
Word[2]: 'file' (flags: 0x0)"

echo ""
echo "=== Operators ==="
run_test "Pipe operator" \
    "ls | grep test" \
    "Word[0]: 'ls' (flags: 0x0)
Word[1]: '|' (flags: 0x4)
Word[2]: 'grep' (flags: 0x0)
Word[3]: 'test' (flags: 0x0)"

run_test "Input redirection" \
    "cat < file.txt" \
    "Word[0]: 'cat' (flags: 0x0)
Word[1]: '<' (flags: 0x0)
Word[2]: 'file.txt' (flags: 0x0)"

run_test "Output redirection" \
    "echo hello > output.txt" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: 'hello' (flags: 0x0)
Word[2]: '>' (flags: 0x0)
Word[3]: 'output.txt' (flags: 0x0)"

run_test "Append redirection" \
    "echo world >> file.txt" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: 'world' (flags: 0x0)
Word[2]: '>>' (flags: 0x0)
Word[3]: 'file.txt' (flags: 0x0)"

run_test "Heredoc" \
    "cat << EOF" \
    "Word[0]: 'cat' (flags: 0x0)
Word[1]: '<<' (flags: 0x0)
Word[2]: 'EOF' (flags: 0x0)"

run_test "AND operator" \
    "make && ./program" \
    "Word[0]: 'make' (flags: 0x0)
Word[1]: '&&' (flags: 0x0)
Word[2]: './program' (flags: 0x0)"

run_test "Multiple pipes" \
    "cat file | grep test | wc -l" \
    "Word[0]: 'cat' (flags: 0x0)
Word[1]: 'file' (flags: 0x0)
Word[2]: '|' (flags: 0x4)
Word[3]: 'grep' (flags: 0x0)
Word[4]: 'test' (flags: 0x0)
Word[5]: '|' (flags: 0x4)
Word[6]: 'wc' (flags: 0x0)
Word[7]: '-l' (flags: 0x0)"

run_test "No spaces around operators" \
    "ls|grep txt>output" \
    "Word[0]: 'ls' (flags: 0x0)
Word[1]: '|' (flags: 0x4)
Word[2]: 'grep' (flags: 0x0)
Word[3]: 'txt' (flags: 0x0)
Word[4]: '>' (flags: 0x0)
Word[5]: 'output' (flags: 0x0)"

echo ""
echo "=== Quotes ==="
run_test "Single quotes" \
    "echo 'hello world'" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: ''hello world'' (flags: 0x2)"

run_test "Double quotes" \
    "echo \"hello world\"" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: '\"hello world\"' (flags: 0x2)"

run_test "Empty single quotes" \
    "echo ''" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: '''' (flags: 0x2)"

run_test "Empty double quotes" \
    "echo \"\"" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: '\"\"' (flags: 0x2)"

run_test "Quotes with special chars" \
    "echo 'hello | world'" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: ''hello | world'' (flags: 0x6)"

run_test "Quotes with redirection chars" \
    "echo \"test > file\"" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: '\"test > file\"' (flags: 0x2)"

echo ""
echo "=== Consecutive Quotes ==="
run_test "Double then single quote" \
    "echo \"hello\"'world'" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: '\"hello\"'world'' (flags: 0x2)"

run_test "Single then double quote" \
    "echo 'hello'\"world\"" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: ''hello'\"world\"' (flags: 0x2)"

run_test "Multiple consecutive quotes" \
    "echo \"part1\"'part2'\"part3\"" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: '\"part1\"'part2'\"part3\"' (flags: 0x2)"

run_test "Quotes with prefix" \
    "echo prefix\"quoted\"" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: 'prefix\"quoted\"' (flags: 0x2)"

run_test "Quotes with suffix" \
    "echo \"quoted\"suffix" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: '\"quoted\"suffix' (flags: 0x2)"

run_test "Quotes with prefix and suffix" \
    "echo pre\"mid\"post" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: 'pre\"mid\"post' (flags: 0x2)"

echo ""
echo "=== Complex Cases ==="
run_test "Complex pipeline" \
    "cat < input.txt | grep 'pattern' | sort > output.txt" \
    "Word[0]: 'cat' (flags: 0x0)
Word[1]: '<' (flags: 0x0)
Word[2]: 'input.txt' (flags: 0x0)
Word[3]: '|' (flags: 0x4)
Word[4]: 'grep' (flags: 0x0)
Word[5]: ''pattern'' (flags: 0x2)
Word[6]: '|' (flags: 0x4)
Word[7]: 'sort' (flags: 0x0)
Word[8]: '>' (flags: 0x0)
Word[9]: 'output.txt' (flags: 0x0)"

run_test "Mixed quotes and operators" \
    "echo \"hello\" | grep 'world' > file.txt" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: '\"hello\"' (flags: 0x2)
Word[2]: '|' (flags: 0x4)
Word[3]: 'grep' (flags: 0x0)
Word[4]: ''world'' (flags: 0x2)
Word[5]: '>' (flags: 0x0)
Word[6]: 'file.txt' (flags: 0x0)"

run_test "Path with quotes" \
    "cd \"/home/user/my documents\"" \
    "Word[0]: 'cd' (flags: 0x0)
Word[1]: '\"/home/user/my documents\"' (flags: 0x2)"

run_test "Variable-like strings" \
    "echo \$USER \$HOME" \
    "Word[0]: 'echo' (flags: 0x0)
Word[1]: '\$USER' (flags: 0x1)
Word[2]: '\$HOME' (flags: 0x1)"

echo ""
echo "=== Edge Cases ==="
run_test "Empty input" \
    "" \
    ""

run_test "Only spaces" \
    "   " \
    ""

run_test "Only operators" \
    "| > < >>" \
    "Word[0]: '|' (flags: 0x4)
Word[1]: '>' (flags: 0x0)
Word[2]: '<' (flags: 0x0)
Word[3]: '>>' (flags: 0x0)"

run_test "Single character" \
    "a" \
    "Word[0]: 'a' (flags: 0x0)"

run_test "Numbers and special chars" \
    "test123 @#% file-name.txt" \
    "Word[0]: 'test123' (flags: 0x0)
Word[1]: '@#%' (flags: 0x0)
Word[2]: 'file-name.txt' (flags: 0x0)"

echo ""
echo "=== Error Cases ==="
run_error_test "Unclosed single quote" \
    "echo 'hello" \
    "unexpected EOF while looking for matching"

run_error_test "Unclosed double quote" \
    "echo \"hello" \
    "unexpected EOF while looking for matching"

run_error_test "Unclosed quote at end" \
    "echo hello \"" \
    "unexpected EOF while looking for matching"

run_error_test "Unclosed quote in middle" \
    "echo 'hello world" \
    "unexpected EOF while looking for matching"

run_error_test "Multiple unclosed quotes" \
    "echo 'hello \"world" \
    "unexpected EOF while looking for matching"

echo ""
echo "========================================="
echo "              TEST SUMMARY"
echo "========================================="
echo "Total tests: $TOTAL_TESTS"
echo -e "Passed: ${GREEN}$PASSED_TESTS${NC}"
echo -e "Failed: ${RED}$FAILED_TESTS${NC}"

if [[ $FAILED_TESTS -eq 0 ]]; then
    echo -e "\n${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "\n${RED}Some tests failed.${NC}"
    exit 1
fi