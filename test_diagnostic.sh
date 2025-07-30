#!/usr/bin/env bash

# Diagnostic test script to understand failures

MINISHELL="./minishell"
TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT

echo "=== Minishell Diagnostic Tests ==="
echo

# Test 1: Basic echo
echo "Test 1: Basic echo"
echo "Command: echo hello"
$MINISHELL -c "echo hello"
echo "Exit code: $?"
echo

# Test 2: Absolute path
echo "Test 2: Absolute path /bin/echo"
echo "Command: /bin/echo hello"
$MINISHELL -c "/bin/echo hello"
echo "Exit code: $?"
echo

# Test 3: Command in PATH
echo "Test 3: Command in PATH (ls)"
echo "Command: ls > /dev/null"
$MINISHELL -c "ls > /dev/null" 2>&1
echo "Exit code: $?"
echo

# Test 4: Single quotes
echo "Test 4: Single quotes"
echo "Command: echo '\$HOME'"
$MINISHELL -c "echo '\$HOME'"
echo "Exit code: $?"
echo

# Test 5: Double quotes
echo "Test 5: Double quotes"
echo "Command: echo \"\$HOME\""
$MINISHELL -c 'echo "$HOME"'
echo "Exit code: $?"
echo

# Test 6: Variable expansion
echo "Test 6: Variable expansion"
echo "Command: echo \$HOME"
$MINISHELL -c 'echo $HOME'
echo "Exit code: $?"
echo

# Test 7: Pipe
echo "Test 7: Pipe"
echo "Command: echo hello | cat"
$MINISHELL -c "echo hello | cat"
echo "Exit code: $?"
echo

# Test 8: Exit status
echo "Test 8: Exit status"
echo "Command: false; echo \$?"
$MINISHELL -c 'false; echo $?'
echo "Exit code: $?"
echo

# Test 9: Export and echo
echo "Test 9: Export and echo"
echo "Command: export TEST=value && echo \$TEST"
$MINISHELL -c 'export TEST=value && echo $TEST'
echo "Exit code: $?"
echo

# Test 10: CD and PWD
echo "Test 10: CD and PWD"
echo "Command: cd /tmp && pwd"
$MINISHELL -c "cd /tmp && pwd"
echo "Exit code: $?"
echo

# Test 11: Input redirection
echo "Test 11: Input redirection"
echo "hello" > $TMPDIR/test.txt
echo "Command: cat < $TMPDIR/test.txt"
$MINISHELL -c "cat < $TMPDIR/test.txt"
echo "Exit code: $?"
echo

# Test 12: HERE document simulation
echo "Test 12: HERE document"
echo "Command: cat << EOF"
printf "cat << EOF\nhello\nEOF\n" | $MINISHELL 2>&1 | grep -v minishell
echo "Exit code: $?"
echo