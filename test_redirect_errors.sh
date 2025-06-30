#!/bin/bash

# Test script for redirect syntax error handling

echo "Testing redirect syntax error handling..."

# Test cases for missing filenames after redirect operators
echo "1. Testing 'echo >' (missing filename)"
echo "echo >" | ./minishell

echo "2. Testing 'cat <' (missing filename)"
echo "cat <" | ./minishell

echo "3. Testing 'echo >>' (missing filename)"
echo "echo >>" | ./minishell

echo "4. Testing 'cat <<' (missing filename)"
echo "cat <<" | ./minishell

# Test cases for invalid redirection targets
echo "5. Testing 'echo > |' (pipe after redirect)"
echo "echo > |" | ./minishell

echo "6. Testing 'echo > >' (redirect after redirect)"
echo "echo > >" | ./minishell

echo "7. Testing 'echo > &&' (AND after redirect)"
echo "echo > &&" | ./minishell

echo "8. Testing 'echo > <' (input redirect after output)"
echo "echo > <" | ./minishell

# Test cases for missing commands after operators
echo "9. Testing 'echo |' (missing command after pipe)"
echo "echo |" | ./minishell

echo "10. Testing 'echo &&' (missing command after AND)"
echo "echo &&" | ./minishell

# Valid cases that should not error
echo "11. Testing 'echo > file.txt' (valid redirect)"
echo "echo > file.txt" | ./minishell

echo "12. Testing 'echo hello | cat' (valid pipe)"
echo "echo hello | cat" | ./minishell

echo "13. Testing 'echo hello && echo world' (valid AND)"
echo "echo hello && echo world" | ./minishell

echo "All redirect error tests completed."