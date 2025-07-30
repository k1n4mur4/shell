#!/usr/bin/env bash

# Test remaining failing tests

echo "=== Testing Remaining Issues ==="

echo "Test 1: Single quote syntax error"
echo 'echo "unclosed' | ./minishell 2>&1 | grep -o "syntax error"
echo "Expected: syntax error"
echo

echo "Test 2: Double quote syntax error"  
echo "echo 'unclosed" | ./minishell 2>&1 | grep -o "syntax error"
echo "Expected: syntax error"
echo

echo "Test 3: Here document"
printf "cat << EOF\nhello\nworld\nEOF\n" | ./minishell 2>&1 | grep -v minishell
echo "Expected: hello world"
echo

echo "Test 4: Signal handling"
timeout 2 bash -c './minishell -c "sleep 5" &
PID=$!
sleep 0.5
kill -INT $PID
wait $PID
echo Exit code: $?'
echo "Expected: Exit code 130"