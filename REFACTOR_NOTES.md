Those are notes about the refactoring to help me think. They should be deleted at some point

## rebuild execution

I need to rebuild execution with parallel execution
I think we can keep the AST structure but just remove the wait in CMD_node
Also need to rebuild how pipes are handled

I have a problem: if a command execution fails, I need to free the pids in the child 

## rebuild pipes


## exit status

Exit status only needs to be tryutn at the end of a line / pipeline
But it can occurs in multiple scenariis

Ok I need to clarify things

the exit status is always the one of the last command except with signals
general error = 1
syntax error = 2 ( and don't execute the rest)
no right to execute = 126
command not found = 127
invalid argument to exit = 128
script terminated by ctrl+C = 130
exit status out of range = 255

## parenthesis

### execution

I have two ideas on how to deal with parenthesis:
1. call minishell inside minishell for the scope of the parenthesis
2. fork the environment and set priorities

Problems
1. when to exit? also seems quite heavy on the processor. How to handle signals?
2. it's not just the environment. For example "cd" changes directories.

-> Okay I know which technique to use. There is another node type SUB. When dfs 
encounters a SUB node, it creates a child process with fork() and executes it's left and right nodes inside the child.
Then the parent process gets the exit status from the one of the child

Problems 2
Actually Sub is conccurential but AND and OR are sequential -> must wait for all the previous processes to end before executing

Problems 3
I get a strange behavior during execution of simple commands like -> (ls)
It is as if the prompt message >minishell was printed before the command could return its execution
It is only with external commands though

AAH okay j'ai compris.
le programme waitpid() a la fin. Mais ici, le process parent ne sait pas qu'un programme est execute dans l'enfant, donc il termine le cycle et retourne a readline(). Je suppose aussi que du coup les infos ne sont pas transmises a la suite de la pipeline
-> I see no solution to this problem. I could use singletons and stuff but I don't want to.

Problems 4
unset HOME && echo $HOME -> doesn't work first try, because && don't wait for builtins
Bash is different in multiple ways
-> it actually creates subprocesses even for builtins
-> it expands variables just before each commands, not at the beginning like me


### parsing

1. Tokenize with parenthesis or with subshell
2. Check syntax
3. Create ast with subshell nodes

correct syntax:

-> (cat Makefile)
-> (cat Makefile | (grep "a")) 
-> (cat Makefile) | (grep "a")
-> echo prout || (ifconfig && echo salut)
-> echo prout | (unset HOME && echo $HOME) | cat
-> (unset HOME) | echo $HOME

wrong syntax:

-> (cat) Makefile
-> echo prout && ()
-> ()
-> echo (prout) 
-> echo prout (|) cat
-> ( echo prout 
-> echo prout | ((echo test))   I think we can consider this as okay
-> "(" echo prout ")"
-> "( echo prout )"
-> '( echo prout )'
-> 
RULES:

-> all opened parenthesis should be closed
-> inside parenthesis cannot be empty
-> inside parenthesis must be a valid command / pipeline

### AST creation

I think a good idea could be for SUBSHELL nodes ot only have left child


