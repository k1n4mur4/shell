#!/bin/bash

# Specialized Test Suite for expr-like operations with $?
# Testing various combinations of exit status usage

MINISHELL="./minishell"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m'

run_expr_test() {
    local test_name="$1"
    local setup_command="$2"
    local test_command="$3"
    local description="$4"
    
    echo -e "${BLUE}Testing: ${test_name}${NC}"
    echo "  Description: $description"
    echo "  Setup: $setup_command"
    echo "  Test: $test_command"
    
    # Full command for minishell
    full_command="$setup_command; $test_command"
    
    # Run in minishell
    minishell_result=$(echo "$full_command" | timeout 5 $MINISHELL 2>&1)
    
    # Run in bash
    bash_result=$(echo "$full_command" | timeout 5 bash 2>&1)
    
    echo "  Minishell: '$minishell_result'"
    echo "  Bash:      '$bash_result'"
    echo
}

main() {
    echo -e "${BLUE}===== Exit Status Expression Tests =====${NC}"
    echo -e "${BLUE}Testing expr-like operations with \$? variable${NC}"
    echo
    
    if [ ! -x "$MINISHELL" ]; then
        echo -e "${RED}Error: minishell executable not found${NC}"
        exit 1
    fi
    
    # Basic $? tests
    echo -e "${YELLOW}=== Basic Exit Status Tests ===${NC}"
    run_expr_test "true_exit_status" "true" "echo \$?" "Exit status of true command"
    run_expr_test "false_exit_status" "false" "echo \$?" "Exit status of false command"
    
    # Multiple $? references (like expr $? + $?)
    echo -e "${YELLOW}=== Multiple Exit Status References ===${NC}"
    run_expr_test "double_exit_status" "false" "echo \$? \$?" "Two references to same exit status"
    run_expr_test "triple_exit_status" "false" "echo \$? \$? \$?" "Three references to same exit status"
    
    # Exit status with arithmetic-like patterns
    echo -e "${YELLOW}=== Arithmetic-like Patterns ===${NC}"
    run_expr_test "exit_status_addition_pattern" "false" "echo \$? + \$?" "Pattern: \$? + \$?"
    run_expr_test "exit_status_multiplication_pattern" "false" "echo \$? * \$?" "Pattern: \$? * \$?"
    run_expr_test "exit_status_subtraction_pattern" "true" "echo \$? - \$?" "Pattern: \$? - \$?"
    
    # Exit status stored in variables
    echo -e "${YELLOW}=== Exit Status in Variables ===${NC}"
    run_expr_test "store_exit_status" "false" "A=\$?; echo \$A" "Store exit status in variable"
    run_expr_test "double_stored_exit_status" "false" "A=\$?; echo \$A \$A" "Use stored exit status twice"
    run_expr_test "store_and_compare" "false" "A=\$?; B=\$?; echo \$A \$B" "Store exit status in two variables"
    
    # Complex scenarios with exit status
    echo -e "${YELLOW}=== Complex Exit Status Scenarios ===${NC}"
    run_expr_test "exit_status_in_string" "false" "echo \"Exit code: \$?\"" "Exit status in string"
    run_expr_test "exit_status_with_other_vars" "false" "echo \"User: \$USER, Exit: \$?\"" "Exit status with other variables"
    run_expr_test "exit_status_concatenation" "false" "echo \$?\$?" "Exit status concatenation without space"
    
    # Exit status after different types of commands
    echo -e "${YELLOW}=== Exit Status After Different Commands ===${NC}"
    run_expr_test "exit_status_after_echo" "echo hello" "echo \$?" "Exit status after echo"
    run_expr_test "exit_status_after_ls" "ls > /dev/null" "echo \$?" "Exit status after ls"
    run_expr_test "exit_status_after_builtin" "pwd > /dev/null" "echo \$?" "Exit status after builtin"
    run_expr_test "exit_status_after_pipe" "echo hello | cat > /dev/null" "echo \$?" "Exit status after pipe"
    
    # Exit status with command substitution patterns
    echo -e "${YELLOW}=== Exit Status with Command Patterns ===${NC}"
    run_expr_test "exit_status_as_arg" "false" "echo The exit code was \$?" "Exit status as argument"
    run_expr_test "exit_status_in_export" "false" "export EXITCODE=\$?; echo \$EXITCODE" "Export exit status"
    run_expr_test "exit_status_in_conditional_pattern" "false" "echo \$? && echo success || echo failure" "Exit status in conditional pattern"
    
    # Nested exit status scenarios
    echo -e "${YELLOW}=== Nested Exit Status Scenarios ===${NC}"
    run_expr_test "multiple_commands_exit_status" "true; false" "echo \$?" "Exit status of last command in sequence"
    run_expr_test "exit_status_after_exit_status" "false; echo \$?" "echo \$?" "Exit status after printing exit status"
    run_expr_test "complex_sequence" "true; false; true" "echo \$?" "Exit status after complex sequence"
    
    # Special cases
    echo -e "${YELLOW}=== Special Exit Status Cases ===${NC}"
    run_expr_test "exit_status_127" "nonexistent_command_xyz 2>/dev/null" "echo \$?" "Exit status 127 (command not found)"
    run_expr_test "exit_status_with_quotes" "false" "echo '\$?' \"\$?\"" "Exit status with different quote types"
    run_expr_test "exit_status_empty_command" "" "echo \$?" "Exit status with no previous command"
    
    echo -e "${GREEN}===== Exit Status Expression Tests Complete =====${NC}"
    echo -e "${YELLOW}Note: These tests compare minishell behavior with bash${NC}"
    echo -e "${YELLOW}Differences may indicate areas for improvement or acceptable variations${NC}"
}

main "$@"