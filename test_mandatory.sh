#!/usr/bin/env bash

# Test script for 42 Minishell mandatory requirements
# Author: Generated test suite
# Version: 8.3

# Colors for output
if [[ -t 1 ]]; then
    RED=$(tput setaf 1)
    GREEN=$(tput setaf 2)
    YELLOW=$(tput setaf 3)
    BLUE=$(tput setaf 4)
    RESET=$(tput sgr0)
else
    RED=""
    GREEN=""
    YELLOW=""
    BLUE=""
    RESET=""
fi

# Global variables
MINISHELL="./minishell"
TMPDIR=""
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Cleanup function
cleanup() {
    if [[ -n "$TMPDIR" && -d "$TMPDIR" ]]; then
        rm -rf "$TMPDIR"
    fi
}

# Set up trap for cleanup
trap cleanup EXIT

# Initialize test environment
init_test_env() {
    TMPDIR=$(mktemp -d)
    if [[ ! -x "$MINISHELL" ]]; then
        echo "${RED}Error: $MINISHELL not found or not executable${RESET}"
        exit 1
    fi
}

# Print test header
print_test_header() {
    local test_num=$1
    local test_name=$2
    printf "[TEST %02d] %-35s " "$test_num" "$test_name"
}

# Print test result
print_result() {
    local status=$1
    if [[ $status -eq 0 ]]; then
        echo "${GREEN}[OK]${RESET}"
        ((PASSED_TESTS++))
    else
        echo "${RED}[KO]${RESET}"
        ((FAILED_TESTS++))
    fi
    ((TOTAL_TESTS++))
}

# Compare minishell output with bash
compare_output() {
    local cmd=$1
    local minishell_out="$TMPDIR/minishell.out"
    local bash_out="$TMPDIR/bash.out"
    
    # Run command in minishell
    "$MINISHELL" -c "$cmd" > "$minishell_out" 2>&1 || true
    local minishell_exit=${PIPESTATUS[0]}
    
    # Run command in bash
    bash -c "$cmd" > "$bash_out" 2>&1 || true
    local bash_exit=${PIPESTATUS[0]}
    
    # Compare outputs and exit codes
    if [[ $minishell_exit -ne $bash_exit ]]; then
        return 1
    fi
    
    if ! diff -q "$minishell_out" "$bash_out" > /dev/null 2>&1; then
        return 1
    fi
    
    return 0
}

# Test 01: Prompt and History
test_01_prompt_and_history() {
    print_test_header 1 "prompt_and_history"
    
    # Since prompt only shows in interactive mode, test basic functionality
    # Test that shell can execute and exit properly
    local output
    output=$(echo "exit" | timeout 2 "$MINISHELL" 2>&1)
    local exit_code=$?
    
    # Check that exit command printed "exit" as expected
    if [[ "$output" == "exit" ]] && [[ $exit_code -eq 0 ]]; then
        print_result 0
        return 0
    else
        print_result 1
        return 1
    fi
}

# Test 02: PATH Resolution
test_02_path_resolution() {
    print_test_header 2 "path_resolution"
    
    # Test absolute path
    if ! compare_output "/bin/echo test"; then
        print_result 1
        return 1
    fi
    
    # Test command in PATH
    if ! compare_output "ls -la > /dev/null"; then
        print_result 1
        return 1
    fi
    
    # Test relative path
    if ! compare_output "./minishell -c 'exit 0'"; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 03: Single Quotes
test_03_single_quote() {
    print_test_header 3 "single_quote"
    
    # Test single quotes prevent expansion
    if ! compare_output "echo '\$HOME'"; then
        print_result 1
        return 1
    fi
    
    # Test unclosed single quote error
    local error_out="$TMPDIR/error.out"
    echo "echo 'unclosed" | "$MINISHELL" 2>"$error_out"
    if ! grep -q "syntax error" "$error_out"; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 04: Double Quotes
test_04_double_quote() {
    print_test_header 4 "double_quote"
    
    # Test double quotes allow variable expansion
    if ! compare_output 'echo "$HOME" | grep -q "/"'; then
        print_result 1
        return 1
    fi
    
    # Test unclosed double quote error
    local error_out="$TMPDIR/error.out"
    echo 'echo "unclosed' | "$MINISHELL" 2>"$error_out"
    if ! grep -q "syntax error" "$error_out"; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 05: Input Redirection
test_05_redirection_input() {
    print_test_header 5 "redirection_input"
    
    # Create test file
    echo "test content" > "$TMPDIR/input.txt"
    
    # Test input redirection
    if ! compare_output "cat < $TMPDIR/input.txt"; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 06: Output Redirection
test_06_redirection_output() {
    print_test_header 6 "redirection_output"
    
    # Test output redirection >
    "$MINISHELL" -c "echo 'test' > $TMPDIR/output.txt" 2>/dev/null
    if [[ ! -f "$TMPDIR/output.txt" ]] || [[ $(cat "$TMPDIR/output.txt") != "test" ]]; then
        print_result 1
        return 1
    fi
    
    # Test append >>
    "$MINISHELL" -c "echo 'append' >> $TMPDIR/output.txt" 2>/dev/null
    if ! grep -q "append" "$TMPDIR/output.txt"; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 07: Here Document
test_07_heredoc() {
    print_test_header 7 "heredoc"
    
    # Test heredoc
    local expected="line1
line2"
    local output
    output=$(printf "cat << EOF\nline1\nline2\nEOF\n" | "$MINISHELL" 2>/dev/null | grep -v "minishell")
    
    if [[ "$output" == "$expected" ]]; then
        print_result 0
        return 0
    else
        print_result 1
        return 1
    fi
}

# Test 08: Pipes
test_08_pipe() {
    print_test_header 8 "pipe"
    
    # Test simple pipe
    if ! compare_output "echo 'hello world' | grep world"; then
        print_result 1
        return 1
    fi
    
    # Test multiple pipes
    if ! compare_output "echo 'test' | cat | cat"; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 09: Environment Variable Expansion
test_09_env_expansion() {
    print_test_header 9 "env_expansion"
    
    # Test environment variable expansion
    export TEST_VAR="test_value"
    if ! compare_output 'echo $TEST_VAR'; then
        print_result 1
        return 1
    fi
    
    # Test non-existent variable
    if ! compare_output 'echo $NONEXISTENT_VAR_12345'; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 10: Exit Status Variable
test_10_exit_status() {
    print_test_header 10 "exit_status"
    
    # Test $? after successful command
    local output
    output=$("$MINISHELL" -c 'true; echo $?' 2>/dev/null)
    if [[ "$output" != "0" ]]; then
        print_result 1
        return 1
    fi
    
    # Test $? after failed command
    output=$("$MINISHELL" -c 'false; echo $?' 2>/dev/null)
    if [[ "$output" != "1" ]]; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 11: Signal Handling
test_11_signal_handling() {
    print_test_header 11 "signal_handling"
    
    # Test Ctrl+C handling (SIGINT) - use timeout to avoid hanging
    local test_script="$TMPDIR/test_sigint.sh"
    cat > "$test_script" << 'EOF'
#!/bin/bash
timeout 3 "$1" -c 'sleep 10' &
PID=$!
sleep 0.5
kill -INT $PID 2>/dev/null
wait $PID 2>/dev/null
echo $?
EOF
    chmod +x "$test_script"
    
    local exit_code
    exit_code=$("$test_script" "$MINISHELL" 2>/dev/null)
    
    # SIGINT should cause exit with code 130, but accept any code indicating signal termination
    if [[ "$exit_code" == "130" ]] || [[ "$exit_code" == "2" ]] || [[ "$exit_code" -gt 128 ]]; then
        print_result 0
        return 0
    else
        print_result 1
        return 1
    fi
}

# Test 12: Builtin echo
test_12_builtin_echo() {
    print_test_header 12 "builtin_echo"
    
    # Test echo without flag
    if ! compare_output "echo hello world"; then
        print_result 1
        return 1
    fi
    
    # Test echo -n
    local output
    output=$("$MINISHELL" -c "echo -n test" 2>/dev/null)
    if [[ "$output" != "test" ]]; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 13: Builtin cd
test_13_builtin_cd() {
    print_test_header 13 "builtin_cd"
    
    # Test cd to tmp directory
    local output
    output=$("$MINISHELL" -c "cd $TMPDIR && pwd" 2>/dev/null)
    if [[ "$output" != "$TMPDIR" ]]; then
        print_result 1
        return 1
    fi
    
    # Test cd without arguments (should go to HOME)
    output=$("$MINISHELL" -c "cd && pwd" 2>/dev/null)
    if [[ "$output" != "$HOME" ]]; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 14: Builtin pwd
test_14_builtin_pwd() {
    print_test_header 14 "builtin_pwd"
    
    # Test pwd
    local current_dir=$(pwd)
    local output
    output=$("$MINISHELL" -c "pwd" 2>/dev/null)
    
    if [[ "$output" == "$current_dir" ]]; then
        print_result 0
        return 0
    else
        print_result 1
        return 1
    fi
}

# Test 15: Builtin export
test_15_builtin_export() {
    print_test_header 15 "builtin_export"
    
    # Test export with value
    local output
    output=$("$MINISHELL" -c "export TEST_EXPORT=value && echo \$TEST_EXPORT" 2>/dev/null)
    if [[ "$output" != "value" ]]; then
        print_result 1
        return 1
    fi
    
    # Test export without value
    "$MINISHELL" -c "export TEST_EMPTY" 2>/dev/null
    local exit_code=$?
    if [[ $exit_code -ne 0 ]]; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 16: Builtin unset
test_16_builtin_unset() {
    print_test_header 16 "builtin_unset"
    
    # Test unset
    local output
    output=$("$MINISHELL" -c "export TEST_UNSET=value && unset TEST_UNSET && echo \$TEST_UNSET" 2>/dev/null)
    if [[ -n "$output" ]]; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 17: Builtin env
test_17_builtin_env() {
    print_test_header 17 "builtin_env"
    
    # Test env command shows PATH
    local output
    output=$("$MINISHELL" -c "env" 2>/dev/null | grep -c "PATH=")
    
    if [[ "$output" -ge 1 ]]; then
        print_result 0
        return 0
    else
        print_result 1
        return 1
    fi
}

# Test 18: Builtin exit
test_18_builtin_exit() {
    print_test_header 18 "builtin_exit"
    
    # Test exit with numeric argument
    "$MINISHELL" -c "exit 42" >/dev/null 2>&1
    local exit_code=$?
    if [[ $exit_code -ne 42 ]]; then
        print_result 1
        return 1
    fi
    
    # Test exit with invalid argument
    local output
    output=$("$MINISHELL" -c "exit abc" 2>&1)
    if ! echo "$output" | grep -q "numeric argument required"; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Main function
main() {
    echo "${BLUE}=== 42 Minishell Mandatory Tests ===${RESET}"
    echo
    
    # Initialize test environment
    init_test_env
    
    # Run all tests
    test_01_prompt_and_history
    test_02_path_resolution
    test_03_single_quote
    test_04_double_quote
    test_05_redirection_input
    test_06_redirection_output
    test_07_heredoc
    test_08_pipe
    test_09_env_expansion
    test_10_exit_status
    test_11_signal_handling
    test_12_builtin_echo
    test_13_builtin_cd
    test_14_builtin_pwd
    test_15_builtin_export
    test_16_builtin_unset
    test_17_builtin_env
    test_18_builtin_exit
    
    # Print summary
    echo
    echo "${BLUE}=============================================${RESET}"
    echo "${GREEN}SUCCESS  : $PASSED_TESTS / $TOTAL_TESTS${RESET}"
    if [[ $FAILED_TESTS -gt 0 ]]; then
        echo "${RED}FAILURE  : $FAILED_TESTS / $TOTAL_TESTS${RESET}"
        exit 1
    else
        echo "${GREEN}All tests passed!${RESET}"
        exit 0
    fi
}

# Run main function if script is executed directly
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi