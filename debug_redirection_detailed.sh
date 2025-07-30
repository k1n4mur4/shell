#!/bin/bash

echo "=== Detailed Redirection Debug Analysis ==="

TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT

echo
echo "1. Testing find_last_redirect() function behavior"
echo "================================================"

# Test multiple output redirections step by step
echo "Test: echo 'content' > file1.txt > file2.txt"
echo

# Create a simple test to see which files get created
./minishell -c "echo 'debug test' > $TMPDIR/test1.txt > $TMPDIR/test2.txt" 2>/dev/null

echo "Files created:"
ls -la $TMPDIR/test*.txt 2>/dev/null || echo "No test files found"

echo
echo "File contents:"
echo "test1.txt: $(cat $TMPDIR/test1.txt 2>/dev/null || echo 'empty/missing')"
echo "test2.txt: $(cat $TMPDIR/test2.txt 2>/dev/null || echo 'empty/missing')"

echo
echo "2. Testing redirection enum values"
echo "=================================="

# Create a small C program to check enum values
cat > $TMPDIR/check_enums.c << 'EOF'
#include <stdio.h>

typedef enum e_redirect_type {
    R_INPUT,
    R_OUTPUT, 
    R_APPEND,
    R_HEREDOC
} t_redirect_type;

int main() {
    printf("R_INPUT = %d\n", R_INPUT);
    printf("R_OUTPUT = %d\n", R_OUTPUT);
    printf("R_APPEND = %d\n", R_APPEND);
    printf("R_HEREDOC = %d\n", R_HEREDOC);
    
    printf("\nBit masks:\n");
    printf("(1 << R_INPUT) = %d\n", (1 << R_INPUT));
    printf("(1 << R_OUTPUT) = %d\n", (1 << R_OUTPUT));
    printf("(1 << R_APPEND) = %d\n", (1 << R_APPEND));
    printf("(1 << R_HEREDOC) = %d\n", (1 << R_HEREDOC));
    
    printf("\nOutput mask: (1 << R_OUTPUT) | (1 << R_APPEND) = %d\n", 
           (1 << R_OUTPUT) | (1 << R_APPEND));
    
    return 0;
}
EOF

gcc $TMPDIR/check_enums.c -o $TMPDIR/check_enums 2>/dev/null
if [[ -f $TMPDIR/check_enums ]]; then
    $TMPDIR/check_enums
else
    echo "Could not compile enum checker"
fi

echo
echo "3. Testing file descriptor behavior"
echo "===================================="

# Test if files are being opened at all
echo "Creating files manually to test FD behavior:"
echo "file1" > $TMPDIR/manual1.txt
echo "file2" > $TMPDIR/manual2.txt

echo "Before minishell command:"
ls -la $TMPDIR/manual*.txt

./minishell -c "echo 'overwrite test' > $TMPDIR/manual1.txt > $TMPDIR/manual2.txt" 2>/dev/null

echo "After minishell command:"
ls -la $TMPDIR/manual*.txt
echo "manual1.txt: $(cat $TMPDIR/manual1.txt)"
echo "manual2.txt: $(cat $TMPDIR/manual2.txt)"

echo
echo "4. Testing input redirection behavior"
echo "====================================="

echo "input1 content" > $TMPDIR/input1.txt
echo "input2 content" > $TMPDIR/input2.txt

echo "Before: input1.txt = $(cat $TMPDIR/input1.txt)"
echo "Before: input2.txt = $(cat $TMPDIR/input2.txt)"

echo "Running: cat < input1.txt < input2.txt"
result=$(./minishell -c "cat < $TMPDIR/input1.txt < $TMPDIR/input2.txt" 2>/dev/null)
echo "Result: '$result'"

echo
echo "5. Testing heredoc behavior in detail"
echo "======================================"

echo "Test: cat << EOF > heredoc_test.txt"
printf "cat << EOF > $TMPDIR/heredoc_test.txt\nline1\nline2\nEOF\n" | ./minishell 2>$TMPDIR/heredoc_error.log

echo "Heredoc file exists: $(test -f $TMPDIR/heredoc_test.txt && echo 'YES' || echo 'NO')"
if [[ -f $TMPDIR/heredoc_test.txt ]]; then
    echo "Heredoc file size: $(wc -c < $TMPDIR/heredoc_test.txt) bytes"
    echo "Heredoc file content:"
    cat -A $TMPDIR/heredoc_test.txt  # Show all characters including control chars
fi

echo
echo "Heredoc error log:"
cat $TMPDIR/heredoc_error.log 2>/dev/null || echo "No error log"

echo
echo "6. Comparing with bash behavior"
echo "==============================="

echo "Bash multiple output:"
bash -c "echo 'bash test' > $TMPDIR/bash1.txt > $TMPDIR/bash2.txt" 2>/dev/null
echo "bash1.txt: $(cat $TMPDIR/bash1.txt 2>/dev/null || echo 'empty/missing')"
echo "bash2.txt: $(cat $TMPDIR/bash2.txt 2>/dev/null || echo 'empty/missing')"

echo
echo "Bash multiple input:"
bash_result=$(bash -c "cat < $TMPDIR/input1.txt < $TMPDIR/input2.txt" 2>/dev/null)
echo "Bash result: '$bash_result'"

echo
echo "Bash heredoc:"
printf "cat << EOF > $TMPDIR/bash_heredoc.txt\nline1\nline2\nEOF\n" | bash 2>/dev/null
echo "Bash heredoc content:"
cat $TMPDIR/bash_heredoc.txt 2>/dev/null || echo "No bash heredoc file"