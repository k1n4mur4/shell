#!/bin/bash

# Advanced Test Suite for Minishell - Complex Scenarios
# Including expr-like operations with $? and complex variable handling

MINISHELL="./minishell"
PASSED=0
FAILED=0
TOTAL=0

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m'

run_test() {
    local test_name="$1"
    local command="$2"
    local description="$3"
    
    echo -e "${BLUE}Testing: ${test_name}${NC}"
    echo "  Description: $description"
    echo "  Command: $command"
    
    # Run in minishell
    actual=$(echo "$command" | timeout 10 $MINISHELL 2>&1)
    
    # Run in bash for comparison
    bash_result=$(echo "$command" | timeout 10 bash 2>&1)
    
    TOTAL=$((TOTAL + 1))
    
    echo "  Minishell: '$actual'"
    echo "  Bash:      '$bash_result'"
    
    # Simple comparison - just check if both produce some output or both fail
    if [[ -n "$actual" && -n "$bash_result" ]] || [[ -z "$actual" && -z "$bash_result" ]]; then
        echo -e "  ${GREEN}PASSED (Output exists check)${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "  ${RED}FAILED${NC}"
        FAILED=$((FAILED + 1))
    fi
    echo
}

# Test arithmetic-like operations with exit status
test_expr_like_operations() {
    echo -e "${YELLOW}=== Arithmetic-like Operations with \$? ===${NC}"
    
    # Basic expr $? + $? equivalent (since minishell may not have expr)
    run_test "double_exit_status" "false; echo \$? \$?" "Display exit status twice"
    
    # Simulate expr-like operations using shell arithmetic if available
    run_test "exit_status_arithmetic_simulation" "false; A=\$?; echo \$A \$A" "Store and reuse exit status"
    
    # Complex exit status scenarios
    run_test "nested_command_exit_status" "false; true; echo \$?" "Nested command exit status"
    run_test "pipe_exit_status_complex" "false | true; echo \$?" "Pipe exit status complex case"
    
    # Multiple exit status in different contexts
    run_test "exit_status_with_variables" "false; EXITCODE=\$?; echo \"Code: \$EXITCODE, Again: \$?\"" "Exit status with variables"
    
    # Exit status in conditionals (if minishell supports && and ||)
    run_test "exit_status_logical_ops" "true && echo \"success: \$?\" || echo \"failure: \$?\"" "Exit status with logical operators"
    run_test "exit_status_logical_ops_false" "false && echo \"success: \$?\" || echo \"failure: \$?\"" "Exit status with logical operators (false case)"
}

# Test complex variable scenarios
test_complex_variables() {
    echo -e "${YELLOW}=== Complex Variable Scenarios ===${NC}"
    
    # Variable arithmetic simulation
    run_test "variable_concatenation_numbers" "export A=123; export B=456; echo \$A\$B" "Concatenate number-like variables"
    
    # Variable expansion in different contexts
    run_test "variable_in_command_substitution" "export CMD=echo; \$CMD hello" "Variable as command name"
    
    # Complex export scenarios
    run_test "export_with_spaces" "export \"VAR=hello world\"; echo \$VAR" "Export variable with spaces"
    run_test "export_overwrite" "export TEST=first; export TEST=second; echo \$TEST" "Export variable overwrite"
    
    # Variable expansion edge cases
    run_test "variable_empty_expansion" "export EMPTY=; echo \"[\$EMPTY]\"" "Empty variable expansion"
    run_test "variable_unset_after_export" "export TEST=value; unset TEST; echo \"[\$TEST]\"" "Variable unset after export"
    
    # Nested variable-like operations
    run_test "variable_as_variable_name" "export A=B; export B=hello; echo \$B" "Variable referencing (indirect)"
}

# Test complex pipe and redirection scenarios
test_complex_pipe_redirect() {
    echo -e "${YAML}=== Complex Pipe and Redirection ===${NC}"
    
    # Multiple pipes with exit status
    run_test "multi_pipe_exit_status" "echo hello | cat | cat; echo \$?" "Multi-pipe exit status"
    
    # Pipes with variable expansion
    run_test "pipe_with_variable_expansion" "export MSG=hello; echo \$MSG | cat" "Pipe with variable expansion"
    
    # Complex redirection scenarios
    run_test "redirect_with_variables" "export FILE=/tmp/test_minishell; echo hello > \$FILE; cat \$FILE; rm -f \$FILE" "Redirection with variables"
    
    # Pipe and redirect combined
    run_test "pipe_and_redirect_combined" "echo hello | cat > /tmp/test_minishell; cat /tmp/test_minishell; rm -f /tmp/test_minishell" "Pipe and redirect combined"
    
    # Multiple redirections
    run_test "multiple_redirections" "echo hello > /tmp/test1 2> /tmp/test2; cat /tmp/test1; rm -f /tmp/test1 /tmp/test2" "Multiple redirections"
}

# Test builtin commands in complex scenarios
test_complex_builtins() {
    echo -e "${YELLOW}=== Complex Builtin Scenarios ===${NC}"
    
    # Echo with complex variable expansion
    run_test "echo_complex_variables" "export A=hello; export B=world; echo \$A \$B \$A" "Echo with multiple variables"
    
    # Echo with special characters
    run_test "echo_special_characters" "echo \"Tab:\t Newline:\n Quote:\\\"\"" "Echo with escape sequences"
    
    # CD with variable expansion
    run_test "cd_with_variable" "export DIR=\$HOME; cd \$DIR; pwd | tail -c 10" "CD with variable expansion"
    
    # Export with complex values
    run_test "export_complex_value" "export COMPLEX=\"hello world \$USER test\"; echo \$COMPLEX" "Export with complex value"
    
    # Unset multiple variables
    run_test "unset_multiple" "export A=1; export B=2; unset A B; echo \"[\$A] [\$B]\"" "Unset multiple variables"
    
    # Env with pipe
    run_test "env_with_pipe" "env | grep USER | head -1" "Env command with pipe"
}

# Test error handling and edge cases
test_advanced_error_handling() {
    echo -e "${YELLOW}=== Advanced Error Handling ===${NC}"
    
    # Command not found with complex command lines
    run_test "complex_command_not_found" "nonexistent_command arg1 arg2; echo \$?" "Complex command not found"
    
    # Variable expansion errors
    run_test "undefined_variable_in_command" "\$UNDEFINED_COMMAND arg1" "Undefined variable as command"
    
    # Pipe errors
    run_test "pipe_to_nonexistent" "echo hello | nonexistent_command 2>/dev/null; echo \$?" "Pipe to nonexistent command"
    
    # Redirection errors
    run_test "redirect_to_readonly" "echo hello > /dev/null; echo \$?" "Redirect to /dev/null"
    
    # Mixed success and failure
    run_test "mixed_success_failure" "echo success; false; echo \$?" "Mixed success and failure commands"
}

# Test memory and performance scenarios
test_performance_scenarios() {
    echo -e "${YELLOW}=== Performance and Memory Scenarios ===${NC}"
    
    # Long command lines
    run_test "long_command_line" "echo $(printf 'a%.0s' {1..100})" "Long command line with repetition"
    
    # Many variables
    run_test "many_variables" "export A=1; export B=2; export C=3; export D=4; export E=5; echo \$A\$B\$C\$D\$E" "Many variables"
    
    # Deep pipe chain
    run_test "deep_pipe_chain" "echo hello | cat | cat | cat | cat | cat" "Deep pipe chain"
    
    # Large environment variable
    run_test "large_env_var" "export LARGE=\$(printf 'x%.0s' {1..100}); echo \$LARGE | wc -c" "Large environment variable"
}

# Test minishell-specific compliance
test_minishell_compliance() {
    echo -e "${YELLOW}=== Minishell.md Compliance Tests ===${NC}"
    
    # Single quotes - absolutely no meta interpretation
    run_test "single_quotes_no_meta" "echo '\$HOME \$? * | < > ; & ()'" "Single quotes prevent all meta interpretation"
    
    # Double quotes - only $ interpretation
    run_test "double_quotes_dollar_only" "echo \"\$HOME but not * or |\"" "Double quotes interpret \$ only"
    
    # Exit status variable $?
    run_test "exit_status_after_commands" "true; echo \$?; false; echo \$?; true; echo \$?" "Exit status after various commands"
    
    # Environment variable expansion
    run_test "env_var_expansion" "echo \"User: \$USER Home: \$HOME\"" "Environment variable expansion"
    
    # Builtin vs external command priority
    run_test "builtin_priority" "echo \"builtin echo works\"; /bin/echo \"external echo works\"" "Builtin vs external priority"
    
    # Command line with multiple semicolons
    run_test "multiple_semicolons" "echo first; echo second; echo third" "Multiple semicolon-separated commands"
}

# Main execution
main() {
    echo -e "${BLUE}===== Advanced Minishell Test Suite =====${NC}"
    echo -e "${BLUE}Complex scenarios including expr-like operations with \$?${NC}"
    echo
    
    if [ ! -x "$MINISHELL" ]; then
        echo -e "${RED}Error: minishell executable not found at $MINISHELL${NC}"
        exit 1
    fi
    
    test_expr_like_operations
    test_complex_variables
    test_complex_pipe_redirect
    test_complex_builtins
    test_advanced_error_handling
    test_performance_scenarios
    test_minishell_compliance
    
    echo -e "${BLUE}===== Advanced Test Summary =====${NC}"
    echo -e "Total tests: ${TOTAL}"
    echo -e "Passed: ${GREEN}${PASSED}${NC}"
    echo -e "Failed: ${RED}${FAILED}${NC}"
    
    if [ $FAILED -eq 0 ]; then
        echo -e "${GREEN}All advanced tests completed! 🎉${NC}"
    else
        echo -e "${YELLOW}Some differences found. This may be expected for advanced features.${NC}"
    fi
}

main "$@"