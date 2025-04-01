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

// error messages are more complicated than I thought
// If external prgrm, the name of the programm is first
// if builtin, bash: or minishell: is first
// Do I need to reset STDIN and STDOUT after?

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

int	exec_cmd(char **args, t_ast_node *node, t_minishell *sh)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)	// need to convert env_list to env_array
	{

	}

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
	return (exec_cmd(args, node, sh));
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
