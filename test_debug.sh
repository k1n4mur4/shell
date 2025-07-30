#!/usr/bin/env bash

# Debug test to understand issues

echo "=== Debug Test ==="

# Test 1: Check if environment is initialized
echo "Test 1: Environment initialization"
echo 'env | wc -l' | ./minishell 2>&1
echo "Expected: > 0 lines"
echo

# Test 2: Check static variable bug in exit_value
echo "Test 2: Exit value bug"
./minishell -c 'false; echo $?'
echo "Expected: 1"
echo

# Test 3: Check PATH variable
echo "Test 3: PATH variable"
./minishell -c 'echo $PATH' | head -1
echo "Expected: non-empty PATH"
echo

# Test 4: Direct check of env function
echo "Test 4: Export and check"
./minishell -c 'export TEST=hello; env | grep TEST'
echo "Expected: TEST=hello"
echo