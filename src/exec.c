/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 19:12:12 by gfontagn          #+#    #+#             */
/*   Updated: 2025/04/01 18:16:50 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>
#include <errno.h>

// error messages are more complicated than I thought
// If external prgrm, the name of the programm is first
// if builtin, bash: or minishell: is first
// Do I need to reset STDIN and STDOUT after?

void error_execution(int ern, char *cmd_name)
{
	if (ern == ENOENT)
		ft_printf("bash: %s: No such file or directory\n", cmd_name);
	else if (ern == EACCES)
		ft_printf("bash: %s: Permission denied\n", cmd_name);
	else if (ern == ENOMEM)
		ft_printf("bash: %s: Cannot allocate memory\n", cmd_name);
	else if (ern == ENOEXEC)
		ft_printf("bash: %s: Exec format error\n", cmd_name);
	else if (ern == E2BIG)
		ft_printf("bash: %s: Argument list too long\n", cmd_name);
	else if (ern == ETXTBSY)
		ft_printf("bash: %s: Text file busy\n", cmd_name);
	else if (ern == EISDIR)
		ft_printf("bash: %s: Is a directory\n", cmd_name);
	else
		ft_printf("bash: %s: %s\n", cmd_name, strerror(ern));
}

int	exec_builtin(char **args, t_ast_node *node, t_minishell *sh)
{
	int	ac;

	args++;
	ac = ft_lstlen(args); 
	if (ft_isequal(str, "echo"))
		sh->last_exit = ft_echo(ac, args)
	if (ft_isequal(str, "pwd"))
		sh->last_exit = ft_pwd();
	if (ft_isequal(str, "cd"))
		sh->last_exit = ft_cd(ac, args, sh);
	if (ft_isequal(str, "export"))
		sh->last_exit = ft_export(ac, args, sh->env_list);
	if (ft_isequal(str, "unset"))
		sh->last_exit = ft_unset(ac, args, sh->env_list);
	if (ft_isequal(str, "env"))
		sh->last_exit = ft_env(sh->env_list);
	if (ft_isequal(str, "exit"))
		ft_exit(ac, args, sh);
	return (0);
}

int	execute_command(char **args, char **envp, t_minishell *sh)
{
	int	saved_er;

	if (execve(args[0], args, envp) < 0)
	{
		saved_er = errno;
		free_all_struct(sh, args, envp);
		error_execution(errno);
	}
}

int	exec_external(char **args, t_ast_node *node, t_minishell *sh)
{
	int	pid;
	char	**envp;
	int	status;

	envp = 	convert_envl_to_array(sh->env_list);// to free if execve fails
	if (!envp)
		return (1);
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
		execute_command(args, envp, sh);
	waitpid(pid, &status, 0);
	sh->last_exit = status;
	exit(status);
}

int	cmd_node(t_ast_node *node, t_minishell *sh)
{
	t_token	**token_list;
	char	**args;
	int	len;

	len = ft_lstlen(node->args);
	token_list = populate_tokens(len, node->args);
	token_list = expand_tokens(token_list, sh, sh->env_list);
	args = expanded_list(len, token_list);
	free_token_list(token_list);
	if (set_redirections(args, node) == 1)
		return (1);
	if (is_builtin(args[0]))
		return (exec_builtin(args, node, sh));
	args[0] = find_path(args[0], sh);
	if (!args[0])
		(free_args(args), return (1));
	return (exec_external(args, node, sh));
}
/*

Before executing a command, we create a pipe for each pipe encountered in the node
Pipes are handled before commands
Redirections have priority over pipes
ex:
command1 > file.txt | command2	-> command2 reads from empty pipe
command1 | command2 < input.txt	-> command2 reads from input.txt and pipe is useless


What are the steps for executing one command?
1. tokenize and expand the args into tk_list
2. creat new args with expanded_list()
3. free tk_list
4. execute with args


Ok now we must think about execution.

So what is the rule like?

We go through the tree following the DFS algo.

1. OPERATORS: AND / OR
- cmd1 && cmd2: run cmd2 if cmd1 succeeds (l_e_s == 0)
- cmd1 || cmd2: run cmd2 if cmd1 fails (l_e_s != 0)

How to implement that into the DFS?
=> instead of using l_e_s, we could have a var node->exit_status
for every command
But how to back-propagate up to the pipe for example?
Don't forget that the command is linear. I think we could safely use l_e_s
! All of this apply only for the right. Left is always ran

2. PIPES
create a pipe each time we encounter a node pipe
Then do like pipex and take into account redirect of commands
* */
