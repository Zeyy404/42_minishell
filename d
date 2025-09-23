yalkhidi@lab1r5s12 exec_minishell % ./minishell
minishell$ /expan
argv111111
idk 2
/expan: No such file or directory
minishell$ echo $?
127
minishell$ /bin/ls/what
argv111111
not dir
/bin/ls/what: Not a directory
minishell$ echo $?
126
minishell$ echo hello
hello
minishell$ echo $?
0
minishell$ echo000 $?
path is set but command not there
echo000: command not found
minishell$ echo $?
127
minishell$ /bin/clear
argv111111
idk 2
/bin/clear: No such file or directory
minishell$ echo $?
127
minishell$ unset PATH
minishell$ cat
path searched but not there unseted
cat: No such file or directory
minishell$ echo $?
127
minishell$ /bin/ls/what
argv111111
not dir
/bin/ls/what: Not a directory
minishell$ echo $?
126
minishell$ /expander
argv111111
idk 2
/expander: No such file or directory
minishell$ echo $?
127
minishell$ ./test.sh
argv111111
idk 1
./test.sh: Exec format error
minishell$ ./t.sh
acces den
./t.sh: Permission denied
minishell$ echo $?
126
minishell$ exit
exit
yalkhidi@lab1