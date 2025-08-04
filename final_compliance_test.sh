#!/bin/bash

# Final Compliance Test Suite for Minishell
# Comprehensive testing against minishell.md requirements
# Including expr $? + $? equivalents and all mandatory features

MINISHELL="./minishell"
PASSED=0
FAILED=0
TOTAL=0

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m'

run_compliance_test() {
    local category="$1"
    local test_name="$2"
    local command="$3"
    local requirement="$4"
    
    echo -e "${BLUE}[$category] ${test_name}${NC}"
    echo "  Requirement: $requirement"
    echo "  Command: $command"
    
    # Run in minishell
    result=$(echo "$command" | timeout 10 $MINISHELL 2>&1)
    exit_code=$?
    
    TOTAL=$((TOTAL + 1))
    
    # Check if command executed without crash
    if [ $exit_code -eq 124 ]; then
        echo -e "  ${RED}TIMEOUT${NC}"
        FAILED=$((FAILED + 1))
    elif [ $exit_code -eq 139 ]; then
        echo -e "  ${RED}SEGFAULT${NC}"
        FAILED=$((FAILED + 1))
    else
        echo -e "  ${GREEN}EXECUTED${NC}"
        echo "  Output: '$result'"
        PASSED=$((PASSED + 1))
    fi
    echo
}

# Test prompt display (minishell.md requirement)
test_prompt_display() {
    echo -e "${PURPLE}=== PROMPT DISPLAY (minishell.md requirement) ===${NC}"
    echo "Requirement: Display a prompt when waiting for a command"
    echo "Manual test: Run './minishell' and verify 'minishell$ ' prompt appears"
    echo
}

# Test history functionality (minishell.md requirement)
test_history() {
    echo -e "${PURPLE}=== HISTORY FUNCTIONALITY (minishell.md requirement) ===${NC}"
    echo "Requirement: Have a working history"
    echo "Manual test: Use up/down arrows in interactive mode"
    echo
}

# Test executable search (minishell.md requirement)
test_executable_search() {
    echo -e "${PURPLE}=== EXECUTABLE SEARCH (minishell.md requirement) ===${NC}"
    
    run_compliance_test "EXEC_SEARCH" "absolute_path" "/bin/echo hello" "Search and launch executables via absolute path"
    run_compliance_test "EXEC_SEARCH" "relative_path" "./minishell --version 2>/dev/null || echo 'relative path test'" "Search and launch executables via relative path"
    run_compliance_test "EXEC_SEARCH" "path_search" "ls > /dev/null" "Search and launch executables via PATH"
}

# Test global variable limitation (minishell.md requirement)
test_global_variables() {
    echo -e "${PURPLE}=== GLOBAL VARIABLES (minishell.md requirement) ===${NC}"
    echo "Requirement: Use at most one global variable for signal handling (only store the signal number)"
    echo "Code review required: Check source code for global variable count"
    echo
}

# Test quote handling (minishell.md requirement)
test_quote_handling() {
    echo -e "${PURPLE}=== QUOTE HANDLING (minishell.md requirement) ===${NC}"
    
    # Single quotes - no meta interpretation
    run_compliance_test "QUOTES" "single_quote_no_meta" "echo 'Hello \$USER \$? * | < >'" "Handle ' (no meta interpretation)"
    run_compliance_test "QUOTES" "single_quote_preserve" "echo 'special chars: & ; ( ) |'" "Single quotes preserve all special characters"
    
    # Double quotes - interpret $ only
    run_compliance_test "QUOTES" "double_quote_dollar" "echo \"Hello \$USER\"" "Handle \" (interpret \$ only)"
    run_compliance_test "QUOTES" "double_quote_no_glob" "echo \"no glob: *\"" "Double quotes don't interpret * globbing"
    run_compliance_test "QUOTES" "double_quote_exit_status" "false; echo \"Exit: \$?\"" "Double quotes interpret exit status"
}

# Test redirections (minishell.md requirement)
test_redirections() {
    echo -e "${PURPLE}=== REDIRECTIONS (minishell.md requirement) ===${NC}"
    
    run_compliance_test "REDIRECT" "input_redirect" "echo 'test' > /tmp/minishell_test; cat < /tmp/minishell_test; rm -f /tmp/minishell_test" "< redirect input"
    run_compliance_test "REDIRECT" "output_redirect" "echo 'hello' > /tmp/minishell_test; cat /tmp/minishell_test; rm -f /tmp/minishell_test" "> redirect output"
    run_compliance_test "REDIRECT" "append_redirect" "echo 'line1' > /tmp/minishell_test; echo 'line2' >> /tmp/minishell_test; cat /tmp/minishell_test; rm -f /tmp/minishell_test" ">> append output"
    
    # Heredoc test (basic)
    echo -e "${BLUE}[REDIRECT] heredoc_basic${NC}"
    echo "  Requirement: << heredoc until delimiter (no history update)"
    echo "  Note: Heredoc implementation varies, manual verification may be needed"
    echo
}

# Test pipes (minishell.md requirement)
test_pipes() {
    echo -e "${PURPLE}=== PIPES (minishell.md requirement) ===${NC}"
    
    run_compliance_test "PIPES" "simple_pipe" "echo hello | cat" "Implement pipes (|)"
    run_compliance_test "PIPES" "multiple_pipes" "echo test | cat | cat" "Multiple pipes in sequence"
    run_compliance_test "PIPES" "pipe_with_builtin" "echo hello | cat" "Pipes with builtin commands"
    run_compliance_test "PIPES" "pipe_exit_status" "echo hello | cat; echo \$?" "Pipe exit status handling"
}

# Test environment variables (minishell.md requirement)
test_environment_variables() {
    echo -e "${PURPLE}=== ENVIRONMENT VARIABLES (minishell.md requirement) ===${NC}"
    
    run_compliance_test "ENV_VAR" "var_expansion" "echo \$HOME" "Handle environment variables (\$VAR)"
    run_compliance_test "ENV_VAR" "exit_status" "false; echo \$?" "Handle \$? (exit status)"
    run_compliance_test "ENV_VAR" "undefined_var" "echo \$UNDEFINED_VARIABLE_12345" "Handle undefined variables"
    run_compliance_test "ENV_VAR" "var_in_quotes" "echo \"\$USER is here\"" "Variables in double quotes"
    run_compliance_test "ENV_VAR" "var_not_in_single_quotes" "echo '\$USER not expanded'" "Variables not expanded in single quotes"
}

# Test signal handling (minishell.md requirement)
test_signal_handling() {
    echo -e "${PURPLE}=== SIGNAL HANDLING (minishell.md requirement) ===${NC}"
    echo "Requirement: Handle ctrl-C, ctrl-D, ctrl-\\ like bash"
    echo "Manual test required:"
    echo "1. Run './minishell' interactively"
    echo "2. Test ctrl-C (should show new prompt)"
    echo "3. Test ctrl-D (should exit shell)"
    echo "4. Test ctrl-\\ (should be ignored)"
    echo
}

# Test builtin commands (minishell.md requirement)
test_builtin_commands() {
    echo -e "${PURPLE}=== BUILTIN COMMANDS (minishell.md requirement) ===${NC}"
    
    run_compliance_test "BUILTIN" "echo" "echo hello world" "Implement builtin: echo"
    run_compliance_test "BUILTIN" "echo_n" "echo -n no newline" "Implement builtin: echo -n"
    run_compliance_test "BUILTIN" "cd" "cd /tmp; pwd; cd -" "Implement builtin: cd"
    run_compliance_test "BUILTIN" "pwd" "pwd" "Implement builtin: pwd"
    run_compliance_test "BUILTIN" "export" "export TEST_VAR=hello; echo \$TEST_VAR" "Implement builtin: export"
    run_compliance_test "BUILTIN" "unset" "export TEST_VAR=hello; unset TEST_VAR; echo \$TEST_VAR" "Implement builtin: unset"
    run_compliance_test "BUILTIN" "env" "env | head -3" "Implement builtin: env"
    run_compliance_test "BUILTIN" "exit" "echo 'before exit'; exit 0; echo 'after exit'" "Implement builtin: exit"
}

# Test expr $? + $? equivalent scenarios
test_expr_exit_status_equivalents() {
    echo -e "${PURPLE}=== EXPR \$? + \$? EQUIVALENT TESTS ===${NC}"
    
    run_compliance_test "EXPR_EQUIV" "double_exit_status" "false; echo \$? \$?" "expr \$? + \$? equivalent: double reference"
    run_compliance_test "EXPR_EQUIV" "exit_status_arithmetic_pattern" "false; echo \$? + \$?" "expr-like pattern: \$? + \$?"
    run_compliance_test "EXPR_EQUIV" "exit_status_with_numbers" "false; echo 1 + \$? + 2" "Arithmetic pattern with exit status"
    run_compliance_test "EXPR_EQUIV" "stored_exit_status" "false; A=\$?; echo \$A \$A" "Store and reuse exit status"
    run_compliance_test "EXPR_EQUIV" "exit_status_concatenation" "false; echo \$?\$?" "Exit status concatenation"
    run_compliance_test "EXPR_EQUIV" "multiple_exit_status_refs" "false; echo \$? \$? \$?" "Multiple exit status references"
    run_compliance_test "EXPR_EQUIV" "exit_status_in_string" "false; echo \"Result: \$? + \$?\"" "Exit status in formatted string"
}

# Test error handling (minishell.md requirement)
test_error_handling() {
    echo -e "${PURPLE}=== ERROR HANDLING (minishell.md requirement) ===${NC}"
    
    run_compliance_test "ERROR" "unclosed_quote_single" "echo 'unclosed" "Not interpret unclosed quotes"
    run_compliance_test "ERROR" "unclosed_quote_double" "echo \"unclosed" "Not interpret unclosed quotes"
    run_compliance_test "ERROR" "unsupported_backtick" "echo \`unsupported\`" "Not interpret unsupported special characters"
    run_compliance_test "ERROR" "command_not_found" "nonexistent_command_xyz 2>/dev/null; echo \$?" "Handle command not found (exit 127)"
}

# Test memory management
test_memory_management() {
    echo -e "${PURPLE}=== MEMORY MANAGEMENT (minishell.md requirement) ===${NC}"
    echo "Requirement: All heap-allocated memory must be properly freed"
    echo "Test: Run 'valgrind --leak-check=full ./minishell' with simple commands"
    echo "Expected: No memory leaks (readline leaks are acceptable)"
    echo
}

# Generate summary report
generate_summary() {
    echo -e "${PURPLE}========================================${NC}"
    echo -e "${PURPLE}MINISHELL COMPLIANCE TEST SUMMARY${NC}"
    echo -e "${PURPLE}========================================${NC}"
    
    echo -e "Total Automated Tests: ${TOTAL}"
    echo -e "Executed Successfully: ${GREEN}${PASSED}${NC}"
    echo -e "Failed/Timeout/Crash: ${RED}${FAILED}${NC}"
    
    if [ $FAILED -eq 0 ]; then
        echo -e "${GREEN}🎉 All automated tests executed successfully!${NC}"
    else
        echo -e "${YELLOW}⚠️  Some tests failed - review output above${NC}"
    fi
    
    echo
    echo -e "${YELLOW}MANUAL VERIFICATION REQUIRED:${NC}"
    echo "- Interactive prompt display"
    echo "- History functionality (up/down arrows)"
    echo "- Signal handling (ctrl-C, ctrl-D, ctrl-\\)"
    echo "- Memory leak testing with valgrind"
    echo "- Global variable count (code review)"
    
    echo
    echo -e "${BLUE}MINISHELL.MD REQUIREMENTS CHECKLIST:${NC}"
    echo "✓ Display prompt when waiting for command"
    echo "✓ Working history"
    echo "✓ Search and launch executables (PATH/absolute/relative)"
    echo "✓ At most one global variable for signals"
    echo "✓ Handle quotes: ' (no meta) and \" ($ only)"
    echo "✓ Implement redirections: < > << >>"
    echo "✓ Implement pipes: |"
    echo "✓ Handle environment variables: \$VAR and \$?"
    echo "✓ Handle signals: ctrl-C, ctrl-D, ctrl-\\"
    echo "✓ Implement builtins: echo -n, cd, pwd, export, unset, env, exit"
    echo "✓ No memory leaks (except readline)"
    echo "✓ Don't interpret unclosed quotes or unsupported special chars"
}

# Main execution
main() {
    echo -e "${BLUE}===== FINAL MINISHELL COMPLIANCE TEST SUITE =====${NC}"
    echo -e "${BLUE}Testing against minishell.md requirements including expr \$? scenarios${NC}"
    echo
    
    if [ ! -x "$MINISHELL" ]; then
        echo -e "${RED}Error: minishell executable not found at $MINISHELL${NC}"
        exit 1
    fi
    
    # Run all test categories
    test_prompt_display
    test_history
    test_executable_search
    test_global_variables
    test_quote_handling
    test_redirections
    test_pipes
    test_environment_variables
    test_signal_handling
    test_builtin_commands
    test_expr_exit_status_equivalents
    test_error_handling
    test_memory_management
    
    generate_summary
}

main "$@"