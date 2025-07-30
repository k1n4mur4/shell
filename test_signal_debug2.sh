#!/bin/bash

echo "=== Signal Test Debug 2 ==="

# Test direct process killing
echo "Testing direct minishell process..."
./minishell -c 'sleep 10' &
PID=$!
echo "Started minishell with PID: $PID"
sleep 0.5
echo "Sending SIGINT to PID: $PID"
kill -INT $PID
wait $PID
result=$?
echo "Exit code: $result"
echo "Expected: 130"

echo
echo "Testing with bash for comparison:"
bash -c 'sleep 10' &
PID2=$!
echo "Started bash with PID: $PID2"
sleep 0.5
echo "Sending SIGINT to PID: $PID2"
kill -INT $PID2
wait $PID2
result2=$?
echo "Bash exit code: $result2"