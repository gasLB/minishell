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

