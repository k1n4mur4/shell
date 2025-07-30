#!/bin/bash

echo "=== Debugging Failed Redirection Tests ==="

TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT

echo
echo "TEST 06: Multiple Output Redirections"
echo "Command: echo 'test content' > file1.txt > file2.txt"
echo

echo "Minishell behavior:"
./minishell -c "echo 'test content' > $TMPDIR/mini_file1.txt > $TMPDIR/mini_file2.txt" 2>/dev/null
echo "file1: $(cat $TMPDIR/mini_file1.txt 2>/dev/null || echo 'empty/missing')"
echo "file2: $(cat $TMPDIR/mini_file2.txt 2>/dev/null || echo 'empty/missing')"

echo
echo "Bash behavior:"
bash -c "echo 'test content' > $TMPDIR/bash_file1.txt > $TMPDIR/bash_file2.txt" 2>/dev/null
echo "file1: $(cat $TMPDIR/bash_file1.txt 2>/dev/null || echo 'empty/missing')"
echo "file2: $(cat $TMPDIR/bash_file2.txt 2>/dev/null || echo 'empty/missing')"

echo
echo "TEST 07: Multiple Input Redirections"
echo "Commands: echo 'file1 content' > input1.txt; echo 'file2 content' > input2.txt; cat < input1.txt < input2.txt"
echo

echo "file1 content" > $TMPDIR/input1.txt
echo "file2 content" > $TMPDIR/input2.txt

echo "Minishell behavior:"
./minishell -c "cat < $TMPDIR/input1.txt < $TMPDIR/input2.txt" 2>/dev/null

echo
echo "Bash behavior:"
bash -c "cat < $TMPDIR/input1.txt < $TMPDIR/input2.txt" 2>/dev/null

echo
echo "TEST 09: Heredoc Output Combination"
echo "Command: cat << EOF > output.txt"

echo "Minishell behavior:"
printf "cat << EOF > $TMPDIR/mini_heredoc.txt\nhello\nworld\nEOF\n" | ./minishell 2>/dev/null
echo "Output file: $(cat $TMPDIR/mini_heredoc.txt 2>/dev/null || echo 'empty/missing')"

echo
echo "Bash behavior:"
printf "cat << EOF > $TMPDIR/bash_heredoc.txt\nhello\nworld\nEOF\n" | bash 2>/dev/null
echo "Output file: $(cat $TMPDIR/bash_heredoc.txt 2>/dev/null || echo 'empty/missing')"

echo
echo "TEST 13: Complex Pipe Redirections"
echo "Command: cat < fruits.txt | grep 'a' > filtered.txt"

echo -e "apple\nbanana\ncherry\ndate" > $TMPDIR/fruits.txt

echo "Minishell behavior:"
./minishell -c "cat < $TMPDIR/fruits.txt | grep 'a' > $TMPDIR/mini_filtered.txt" 2>/dev/null
echo "Filtered: $(cat $TMPDIR/mini_filtered.txt 2>/dev/null || echo 'empty/missing')"

echo
echo "Bash behavior:"
bash -c "cat < $TMPDIR/fruits.txt | grep 'a' > $TMPDIR/bash_filtered.txt" 2>/dev/null
echo "Filtered: $(cat $TMPDIR/bash_filtered.txt 2>/dev/null || echo 'empty/missing')"