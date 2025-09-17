#!/bin/bash

# Test script for your custom minishell

echo "Testing simple commands"
echo "Hello, world!"
pwd
ls -la

echo "Testing redirections"
echo "This should go to a file" > test_output.txt
cat test_output.txt

echo "Testing pipes"
echo "Hello Pipe" | grep Pipe

echo "Testing AND/OR operators"
echo "first command" && echo "second command"
false || echo "third command after failure"

echo "Cleaning up"
rm -f test_output.txt

echo "All tests done!"
