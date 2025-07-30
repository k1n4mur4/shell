#!/usr/bin/env bash

# Comprehensive test suite for redirection combinations
# Tests: > < >> << and their combinations

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
    printf "[TEST %02d] %-40s " "$test_num" "$test_name"
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

# Compare files content
compare_files() {
    local file1=$1
    local file2=$2
    
    if [[ ! -f "$file1" ]] || [[ ! -f "$file2" ]]; then
        return 1
    fi
    
    if ! diff -q "$file1" "$file2" > /dev/null 2>&1; then
        return 1
    fi
    
    return 0
}

# Compare minishell and bash behavior
compare_behavior() {
    local cmd=$1
    local minishell_out="$TMPDIR/minishell.out"
    local minishell_err="$TMPDIR/minishell.err"
    local bash_out="$TMPDIR/bash.out"  
    local bash_err="$TMPDIR/bash.err"
    
    # Run command in minishell
    $MINISHELL -c "$cmd" > "$minishell_out" 2> "$minishell_err" || true
    local minishell_exit=$?
    
    # Run command in bash
    bash -c "$cmd" > "$bash_out" 2> "$bash_err" || true
    local bash_exit=$?
    
    # Compare outputs and exit codes
    if [[ $minishell_exit -ne $bash_exit ]]; then
        return 1
    fi
    
    if ! compare_files "$minishell_out" "$bash_out"; then
        return 1
    fi
    
    return 0
}

# Test 01: Basic Output Redirection
test_01_basic_output() {
    print_test_header 1 "basic_output_redirection"
    
    # Test basic output redirection
    $MINISHELL -c "echo 'hello world' > $TMPDIR/output1.txt" 2>/dev/null
    if [[ ! -f "$TMPDIR/output1.txt" ]] || [[ "$(cat "$TMPDIR/output1.txt")" != "hello world" ]]; then
        print_result 1
        return 1
    fi
    
    # Test overwrite
    $MINISHELL -c "echo 'overwritten' > $TMPDIR/output1.txt" 2>/dev/null
    if [[ "$(cat "$TMPDIR/output1.txt")" != "overwritten" ]]; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 02: Basic Input Redirection
test_02_basic_input() {
    print_test_header 2 "basic_input_redirection"
    
    # Create test input file
    echo -e "line1\nline2\nline3" > "$TMPDIR/input1.txt"
    
    # Test input redirection
    if ! compare_behavior "cat < $TMPDIR/input1.txt"; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 03: Basic Append Redirection
test_03_basic_append() {
    print_test_header 3 "basic_append_redirection"
    
    # Test append redirection
    $MINISHELL -c "echo 'first line' > $TMPDIR/append1.txt" 2>/dev/null
    $MINISHELL -c "echo 'second line' >> $TMPDIR/append1.txt" 2>/dev/null
    
    local expected="first line
second line"
    
    if [[ "$(cat "$TMPDIR/append1.txt")" != "$expected" ]]; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 04: Input + Output Combination
test_04_input_output_combo() {
    print_test_header 4 "input_output_combination"
    
    # Create source file
    echo -e "hello\nworld\ntest" > "$TMPDIR/source.txt"
    
    # Test input + output redirection
    if ! compare_behavior "cat < $TMPDIR/source.txt > $TMPDIR/dest.txt"; then
        print_result 1
        return 1
    fi
    
    # Verify file was created correctly
    if ! compare_files "$TMPDIR/source.txt" "$TMPDIR/dest.txt"; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 05: Input + Append Combination
test_05_input_append_combo() {
    print_test_header 5 "input_append_combination"
    
    # Create source and destination files
    echo -e "line1\nline2" > "$TMPDIR/source2.txt"
    echo "existing content" > "$TMPDIR/dest2.txt"
    
    # Test input + append redirection
    $MINISHELL -c "cat < $TMPDIR/source2.txt >> $TMPDIR/dest2.txt" 2>/dev/null
    
    local expected="existing content
line1
line2"
    
    if [[ "$(cat "$TMPDIR/dest2.txt")" != "$expected" ]]; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 06: Multiple Output Redirections
test_06_multiple_output() {
    print_test_header 6 "multiple_output_redirections"
    
    # Test multiple output redirections - last one should win
    $MINISHELL -c "echo 'test content' > $TMPDIR/file1.txt > $TMPDIR/file2.txt" 2>/dev/null
    
    # file1.txt should be empty or not exist, file2.txt should have content
    if [[ -s "$TMPDIR/file1.txt" ]]; then
        print_result 1
        return 1
    fi
    
    if [[ "$(cat "$TMPDIR/file2.txt" 2>/dev/null)" != "test content" ]]; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 07: Multiple Input Redirections
test_07_multiple_input() {
    print_test_header 7 "multiple_input_redirections"
    
    # Create test files
    echo "file1 content" > "$TMPDIR/input1.txt"
    echo "file2 content" > "$TMPDIR/input2.txt"
    
    # Test multiple input redirections - last one should win
    $MINISHELL -c "cat < $TMPDIR/input1.txt < $TMPDIR/input2.txt > $TMPDIR/result.txt" 2>/dev/null
    
    if [[ "$(cat "$TMPDIR/result.txt")" != "file2 content" ]]; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 08: Mixed Redirection Types
test_08_mixed_redirections() {
    print_test_header 8 "mixed_redirection_types"
    
    # Create source file
    echo "original" > "$TMPDIR/mixed.txt"
    
    # Test mixed redirections: append then overwrite
    $MINISHELL -c "echo 'appended' >> $TMPDIR/mixed.txt > $TMPDIR/final.txt" 2>/dev/null
    
    # mixed.txt should have original + appended, final.txt should have appended
    local mixed_expected="original
appended"
    
    if [[ "$(cat "$TMPDIR/mixed.txt")" != "$mixed_expected" ]]; then
        print_result 1
        return 1
    fi
    
    if [[ "$(cat "$TMPDIR/final.txt")" != "appended" ]]; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 09: Heredoc with Output Redirection
test_09_heredoc_output() {
    print_test_header 9 "heredoc_output_combination"
    
    # Test heredoc with output redirection
    local test_script="$TMPDIR/heredoc_test.sh"
    cat > "$test_script" << 'EOF'
#!/bin/bash
"$1" -c 'cat << DELIMITER > '"$2"'/heredoc_out.txt
first line
second line
third line
DELIMITER'
EOF
    chmod +x "$test_script"
    
    # Run the test
    "$test_script" "$MINISHELL" "$TMPDIR" 2>/dev/null
    
    local expected="first line
second line
third line"
    
    if [[ -f "$TMPDIR/heredoc_out.txt" ]] && [[ "$(cat "$TMPDIR/heredoc_out.txt")" == "$expected" ]]; then
        print_result 0
        return 0
    else
        print_result 1
        return 1
    fi
}

# Test 10: Heredoc with Append Redirection
test_10_heredoc_append() {
    print_test_header 10 "heredoc_append_combination"
    
    # Create existing file
    echo "existing line" > "$TMPDIR/heredoc_append.txt"
    
    # Test heredoc with append redirection
    local test_script="$TMPDIR/heredoc_append_test.sh"
    cat > "$test_script" << 'EOF'
#!/bin/bash
"$1" -c 'cat << END >> '"$2"'/heredoc_append.txt
appended line 1
appended line 2
END'
EOF
    chmod +x "$test_script"
    
    # Run the test
    "$test_script" "$MINISHELL" "$TMPDIR" 2>/dev/null
    
    local expected="existing line
appended line 1
appended line 2"
    
    if [[ -f "$TMPDIR/heredoc_append.txt" ]] && [[ "$(cat "$TMPDIR/heredoc_append.txt")" == "$expected" ]]; then
        print_result 0
        return 0
    else
        print_result 1
        return 1
    fi
}

# Test 11: Error Cases - Non-existent Input File
test_11_error_nonexistent_input() {
    print_test_header 11 "error_nonexistent_input"
    
    # Test reading from non-existent file
    $MINISHELL -c "cat < $TMPDIR/nonexistent.txt" 2>"$TMPDIR/error.out"
    local exit_code=$?
    
    # Should fail with non-zero exit code and error message
    if [[ $exit_code -eq 0 ]]; then
        print_result 1
        return 1
    fi
    
    if [[ ! -s "$TMPDIR/error.out" ]]; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 12: Error Cases - Permission Denied
test_12_error_permission_denied() {
    print_test_header 12 "error_permission_denied"
    
    # Create read-only directory (if possible)
    mkdir -p "$TMPDIR/readonly" 2>/dev/null
    chmod 444 "$TMPDIR/readonly" 2>/dev/null
    
    # Test writing to read-only directory
    $MINISHELL -c "echo 'test' > $TMPDIR/readonly/output.txt" 2>"$TMPDIR/perm_error.out"
    local exit_code=$?
    
    # Restore permissions for cleanup
    chmod 755 "$TMPDIR/readonly" 2>/dev/null
    
    # Should fail with non-zero exit code
    if [[ $exit_code -eq 0 ]]; then
        print_result 1
        return 1
    fi
    
    print_result 0
    return 0
}

# Test 13: Complex Scenario - Pipe with Redirections
test_13_complex_pipe_redirections() {
    print_test_header 13 "complex_pipe_redirections"
    
    # Create input file
    echo -e "apple\nbanana\ncherry\ndate" > "$TMPDIR/fruits.txt"
    
    # Test pipe with input and output redirections
    if compare_behavior "cat < $TMPDIR/fruits.txt | grep 'a' > $TMPDIR/filtered.txt"; then
        # Verify the output file - should contain lines with 'a'
        local expected="apple
banana"
        if [[ "$(cat "$TMPDIR/filtered.txt")" == "$expected" ]]; then
            print_result 0
            return 0
        fi
    fi
    
    print_result 1
    return 1
}

# Test 14: Edge Case - Empty Redirections
test_14_edge_case_empty() {
    print_test_header 14 "edge_case_empty_redirections"
    
    # Test empty input redirection
    touch "$TMPDIR/empty.txt"
    
    if compare_behavior "cat < $TMPDIR/empty.txt > $TMPDIR/empty_out.txt"; then
        # Both files should be empty
        if [[ ! -s "$TMPDIR/empty_out.txt" ]]; then
            print_result 0
            return 0
        fi
    fi
    
    print_result 1
    return 1
}

# Test 15: Redirection Order Independence
test_15_redirection_order() {
    print_test_header 15 "redirection_order_independence"
    
    # Create input file
    echo -e "test\ndata\nhere" > "$TMPDIR/order_input.txt"
    
    # Test different orders of input/output redirections
    $MINISHELL -c "cat < $TMPDIR/order_input.txt > $TMPDIR/order_out1.txt" 2>/dev/null
    $MINISHELL -c "cat > $TMPDIR/order_out2.txt < $TMPDIR/order_input.txt" 2>/dev/null
    
    # Both output files should be identical
    if compare_files "$TMPDIR/order_out1.txt" "$TMPDIR/order_out2.txt"; then
        print_result 0
        return 0
    fi
    
    print_result 1
    return 1
}

# Main function
main() {
    echo "${BLUE}=== Redirection Combinations Test Suite ===${RESET}"
    echo
    
    # Initialize test environment
    init_test_env
    
    # Run all tests
    test_01_basic_output
    test_02_basic_input
    test_03_basic_append
    test_04_input_output_combo
    test_05_input_append_combo
    test_06_multiple_output
    test_07_multiple_input
    test_08_mixed_redirections
    test_09_heredoc_output
    test_10_heredoc_append
    test_11_error_nonexistent_input
    test_12_error_permission_denied
    test_13_complex_pipe_redirections
    test_14_edge_case_empty
    test_15_redirection_order
    
    # Print summary
    echo
    echo "${BLUE}=================================================${RESET}"
    echo "${GREEN}SUCCESS  : $PASSED_TESTS / $TOTAL_TESTS${RESET}"
    if [[ $FAILED_TESTS -gt 0 ]]; then
        echo "${RED}FAILURE  : $FAILED_TESTS / $TOTAL_TESTS${RESET}"
        exit 1
    else
        echo "${GREEN}All redirection combination tests passed!${RESET}"
        exit 0
    fi
}

# Run main function if script is executed directly
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi