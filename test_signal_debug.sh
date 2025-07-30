#!/bin/bash

echo "=== Signal Test Debug ==="

# Test the exact scenario from the test
cat > /tmp/test_sigint.sh << 'EOF'
#!/bin/bash
"$1" -c 'sleep 10' &
PID=$!
sleep 0.5
kill -INT $PID
wait $PID
echo $?
EOF

chmod +x /tmp/test_sigint.sh

echo "Running signal test..."
result=$(/tmp/test_sigint.sh ./minishell 2>/dev/null)
echo "Exit code: $result"
echo "Expected: 130"

# Test with bash for comparison
echo
echo "Testing with bash:"
result2=$(/tmp/test_sigint.sh bash 2>/dev/null)
echo "Bash exit code: $result2"

rm /tmp/test_sigint.sh