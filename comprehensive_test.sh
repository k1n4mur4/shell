#!/bin/bash

# Comprehensive Test Suite for Minishell
# Based on minishell.md specifications

MINISHELL="./minishell"
PASSED=0
FAILED=0
TOTAL=0

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Helper function to run test
run_test() {
    local test_name="$1"
    local command="$2"
    local expected="$3"
    local description="$4"
    
    echo -e "${BLUE}Testing: ${test_name}${NC}"
    if [ -n "$description" ]; then
        echo "  Description: $description"
    fi
    
    # Run command in minishell
    actual=$(echo "$command" | timeout 5 $MINISHELL 2>&1)
    local minishell_exit=$?
    
    # Run command in bash for comparison
    bash_result=$(echo "$command" | timeout 5 bash 2>&1)
    local bash_exit=$?
    
    TOTAL=$((TOTAL + 1))
    
    if [ "$actual" = "$expected" ] || [ "$actual" = "$bash_result" ]; then
        echo -e "  ${GREEN}PASSED${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "  ${RED}FAILED${NC}"
        echo "    Command: $command"
        echo "    Expected: '$expected'"
        echo "    Got:      '$actual'"
        echo "    Bash:     '$bash_result'"
        FAILED=$((FAILED + 1))
    fi
    echo
}

# Test exit status variable $?
test_exit_status() {
    echo -e "${YELLOW}=== Exit Status Tests (\$?) ===${NC}"
    
    run_test "basic_exit_status" "true; echo \$?" "0" "Test exit status after true command"
    run_test "false_exit_status" "false; echo \$?" "1" "Test exit status after false command"
    run_test "command_not_found_status" "nonexistent_command 2>/dev/null; echo \$?" "127" "Test exit status for command not found"
    run_test "echo_exit_status" "echo hello; echo \$?" "0" "Test exit status after successful echo"
    
    # Multiple $? references
    run_test "multiple_exit_status" "false; echo \$? \$?" "1 1" "Test multiple references to exit status"
    
    # $? in different contexts
    run_test "exit_status_in_quotes" "true; echo '\$?'" "\$?" "Test \$? in single quotes (no expansion)"
    run_test "exit_status_in_double_quotes" "false; echo \"\$?\"" "1" "Test \$? in double quotes (expansion)"
}

# Test environment variables
test_environment_variables() {
    echo -e "${YELLOW}=== Environment Variable Tests ===${NC}"
    
    run_test "home_variable" "echo \$HOME" "$HOME" "Test HOME environment variable"
    run_test "path_variable" "echo \$PATH | wc -c" "$(echo $PATH | wc -c)" "Test PATH variable exists"
    run_test "undefined_variable" "echo \$UNDEFINED_VAR_123" "" "Test undefined environment variable"
    
    # Variable expansion in different contexts
    run_test "var_in_single_quotes" "echo '\$HOME'" "\$HOME" "Test variable in single quotes (no expansion)"
    run_test "var_in_double_quotes" "echo \"\$HOME\"" "$HOME" "Test variable in double quotes (expansion)"
    
    # Complex variable scenarios
    run_test "var_concatenation" "echo \$USER\$HOME | head -c 20" "$(echo $USER$HOME | head -c 20)" "Test variable concatenation"
    run_test "var_with_text" "echo prefix\$USER" "prefix$USER" "Test variable with prefix text"
}

# Test export and unset builtins
test_export_unset() {
    echo -e "${YELLOW}=== Export/Unset Tests ===${NC}"
    
    run_test "export_new_var" "export TEST_VAR=hello; echo \$TEST_VAR" "hello" "Test export new variable"
    run_test "export_existing_var" "export PATH=\$PATH:/test; echo \$PATH | grep /test" "$(export PATH=$PATH:/test; echo $PATH | grep /test)" "Test export existing variable"
    run_test "unset_variable" "export TEST_VAR=hello; unset TEST_VAR; echo \$TEST_VAR" "" "Test unset variable"
    
    # Export without value
    run_test "export_no_value" "export TEST_EMPTY; echo \"[\$TEST_EMPTY]\"" "[]" "Test export without value"
}

# Test quote processing (critical for minishell.md compliance)
test_quote_processing() {
    echo -e "${YELLOW}=== Quote Processing Tests ===${NC}"
    
    # Single quotes - no meta interpretation
    run_test "single_quote_no_expansion" "echo 'Hello \$USER world'" "Hello \$USER world" "Single quotes prevent all expansion"
    run_test "single_quote_special_chars" "echo '* | < > & ; ()'" "* | < > & ; ()" "Single quotes preserve special characters"
    run_test "single_quote_with_spaces" "echo 'hello    world'" "hello    world" "Single quotes preserve multiple spaces"
    
    # Double quotes - interpret $ only
    run_test "double_quote_var_expansion" "echo \"Hello \$USER\"" "Hello $USER" "Double quotes allow variable expansion"
    run_test "double_quote_exit_status" "true; echo \"Exit: \$?\"" "Exit: 0" "Double quotes allow exit status expansion"
    run_test "double_quote_preserve_spaces" "echo \"hello    world\"" "hello    world" "Double quotes preserve spaces"
    
    # Mixed quotes
    run_test "mixed_quotes" "echo 'single' \"double\" mixed" "single double mixed" "Test mixed quote types"
    run_test "nested_concept" "echo \"outer 'inner' outer\"" "outer 'inner' outer" "Test quotes within quotes"
}

# Test pipes
test_pipes() {
    echo -e "${YELLOW}=== Pipe Tests ===${NC}"
    
    run_test "simple_pipe" "echo hello | cat" "hello" "Simple pipe test"
    run_test "multiple_pipes" "echo hello | cat | cat" "hello" "Multiple pipes test"
    run_test "pipe_with_builtin" "echo test | cat" "test" "Pipe with builtin command"
    run_test "pipe_exit_status" "echo hello | cat; echo \$?" "hello\n0" "Pipe exit status"
    
    # Complex pipe scenarios
    run_test "pipe_with_variables" "echo \$USER | cat" "$USER" "Pipe with variable expansion"
    run_test "false_pipe_true" "false | true; echo \$?" "0" "Pipe exit status with false|true"
}

# Test redirections
test_redirections() {
    echo -e "${YELLOW}=== Redirection Tests ===${NC}"
    
    # Output redirection
    run_test "output_redirect" "echo hello > /tmp/minishell_test; cat /tmp/minishell_test; rm -f /tmp/minishell_test" "hello" "Output redirection"
    run_test "append_redirect" "echo line1 > /tmp/minishell_test; echo line2 >> /tmp/minishell_test; cat /tmp/minishell_test; rm -f /tmp/minishell_test" "line1\nline2" "Append redirection"
    
    # Input redirection
    run_test "input_redirect" "echo testfile > /tmp/minishell_test; cat < /tmp/minishell_test; rm -f /tmp/minishell_test" "testfile" "Input redirection"
    
    # Heredoc (basic test)
    run_test "heredoc_basic" "cat << EOF\nhello world\nEOF" "hello world" "Basic heredoc test"
}

# Test builtin commands
test_builtins() {
    echo -e "${YELLOW}=== Builtin Command Tests ===${NC}"
    
    # echo tests
    run_test "echo_simple" "echo hello world" "hello world" "Simple echo command"
    run_test "echo_no_newline" "echo -n hello" "hello" "Echo without newline"
    run_test "echo_with_vars" "echo Hello \$USER" "Hello $USER" "Echo with variable"
    
    # pwd test
    run_test "pwd_command" "pwd" "$(pwd)" "PWD builtin command"
    
    # cd tests (using subshell to avoid changing directory)
    run_test "cd_home" "(cd; pwd)" "$HOME" "CD to home directory"
    run_test "cd_relative" "(cd ..; basename \$(pwd))" "$(cd ..; basename $(pwd))" "CD with relative path"
    
    # env test
    run_test "env_command" "env | grep USER | head -1" "$(env | grep USER | head -1)" "ENV builtin command"
    
    # exit test with status
    run_test "exit_with_status" "exit 42; echo \$?" "" "Exit with specific status"
}

# Test error cases
test_error_cases() {
    echo -e "${YELLOW}=== Error Handling Tests ===${NC}"
    
    # Unclosed quotes
    run_test "unclosed_single_quote" "echo 'unclosed" "" "Unclosed single quote error"
    run_test "unclosed_double_quote" "echo \"unclosed" "" "Unclosed double quote error"
    
    # Invalid commands
    run_test "command_not_found" "nonexistent_command_xyz 2>/dev/null; echo \$?" "127" "Command not found exit status"
    
    # Syntax errors
    run_test "pipe_syntax_error" "echo hello |; echo \$?" "" "Pipe syntax error"
    run_test "redirect_syntax_error" "echo hello >; echo \$?" "" "Redirect syntax error"
}

# Test complex scenarios combining multiple features
test_complex_scenarios() {
    echo -e "${YELLOW}=== Complex Scenario Tests ===${NC}"
    
    # expr $? + $? equivalent scenarios
    run_test "arithmetic_like_exit_status" "false; echo \$? \$?" "1 1" "Multiple exit status references"
    run_test "exit_status_in_command" "false; echo \"Exit code: \$?\"" "Exit code: 1" "Exit status in string"
    
    # Combined features
    run_test "pipe_with_redirect" "echo hello | cat > /tmp/minishell_test; cat /tmp/minishell_test; rm -f /tmp/minishell_test" "hello" "Pipe with redirect"
    run_test "var_expansion_with_pipe" "echo \$USER | cat" "$USER" "Variable expansion with pipe"
    run_test "quotes_with_pipe" "echo 'hello world' | cat" "hello world" "Quotes with pipe"
    
    # Multiple commands with different exit statuses
    run_test "multiple_commands_exit" "true; false; echo \$?" "1" "Multiple commands exit status"
    run_test "builtin_external_mix" "echo hello; /bin/echo world; echo \$?" "hello\nworld\n0" "Mix of builtin and external commands"
    
    # Environment variable complex scenarios
    run_test "export_with_expansion" "export TEST=\$USER; echo \$TEST" "$USER" "Export with variable expansion"
    run_test "nested_var_expansion" "export A=hello; export B=\$A; echo \$B" "hello" "Nested variable expansion"
}

# Test special variables and edge cases
test_special_cases() {
    echo -e "${YELLOW}=== Special Cases and Edge Cases ===${NC}"
    
    # $$ (process ID) - if implemented
    run_test "process_id_var" "echo \$\$ | grep -E '^[0-9]+\$' | wc -l" "1" "Process ID variable format"
    
    # Empty commands
    run_test "empty_command" "" "" "Empty command input"
    run_test "whitespace_only" "   " "" "Whitespace only input"
    
    # Special characters in quotes
    run_test "special_chars_single" "echo '\$PATH \$? * | < >'" "\$PATH \$? * | < >" "Special chars in single quotes"
    run_test "special_chars_double" "echo \"\$USER is here\"" "$USER is here" "Variable in double quotes with text"
    
    # Long variable expansion
    run_test "long_path_var" "echo \$PATH | wc -c" "$(echo $PATH | wc -c)" "Long PATH variable"
    
    # Multiple variable expansions
    run_test "multiple_var_expansion" "echo \$USER:\$HOME | cut -d: -f1" "$USER" "Multiple variables in one command"
}

# Main test execution
main() {
    echo -e "${BLUE}===== Comprehensive Minishell Test Suite =====${NC}"
    echo -e "${BLUE}Based on minishell.md specifications${NC}"
    echo
    
    # Check if minishell exists
    if [ ! -x "$MINISHELL" ]; then
        echo -e "${RED}Error: minishell executable not found at $MINISHELL${NC}"
        exit 1
    fi
    
    # Run all test categories
    test_exit_status
    test_environment_variables
    test_export_unset
    test_quote_processing
    test_pipes
    test_redirections
    test_builtins
    test_error_cases
    test_complex_scenarios
    test_special_cases
    
    # Final summary
    echo -e "${BLUE}===== Test Summary =====${NC}"
    echo -e "Total tests: ${TOTAL}"
    echo -e "Passed: ${GREEN}${PASSED}${NC}"
    echo -e "Failed: ${RED}${FAILED}${NC}"
    
    if [ $FAILED -eq 0 ]; then
        echo -e "${GREEN}All tests passed! 🎉${NC}"
        exit 0
    else
        echo -e "${RED}Some tests failed. Please check the output above.${NC}"
        exit 1
    fi
}

# Run main function
main "$@"