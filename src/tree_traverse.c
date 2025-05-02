/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_traverse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 16:39:59 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/01 18:11:17 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

void	dfs_ast(t_ast_node *node, t_minishell *sh)
{
	node->visited = 1;
	if (node->type == PIPE)
		pipe_node(node, sh);
	else if (node->type == CMD)
		cmd_node(node, sh);
	else if (node->type == AND)
	{
		if (node->left && !(node->left->visited))
			dfs_ast(node->left, sh);
		if (!sh->last_exit && node->right && !(node->right->visited))
			dfs_ast(node->right, sh);
	}
	else if (node->type == OR)
	{
		if (node->left && !(node->left->visited))
			dfs_ast(node->left, sh);
		if (sh->last_exit && node->right && !(node->right->visited))
			dfs_ast(node->right, sh);
	}
}

void	add_pipe_fd(int fd1, int fd2, t_minishell *sh)
{
	if (sh->pipe_count + 2 >= MAX_FD)
	{
		printf_fd(2, "Error: Too many open pipes\n");
		return ;
	}
	if (!sh->pipe_fds)
	{
		sh->pipe_fds = malloc(MAX_FD * sizeof(int));
		if (!sh->pipe_fds)
			return ;
		sh->pipe_count = 0;
	}
	sh->pipe_fds[sh->pipe_count++] = fd1;
	sh->pipe_fds[sh->pipe_count++] = fd2;
}

void	pipe_node(t_ast_node *node, t_minishell *sh)
{
	int	fd[2];
	int	or_stdin;
	int	or_stdout;

	or_stdin = dup(STDIN_FILENO);
	or_stdout = dup(STDOUT_FILENO);
	if (pipe(fd) == -1)
		return ;
	(add_pipe_fd(fd[0], fd[1], sh), add_pipe_fd(or_stdin, or_stdout, sh));
	if (node->left && !(node->left->visited))
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		dfs_ast(node->left, sh);
		dup2(or_stdout, STDOUT_FILENO);
	}
	if (node->right && !(node->right->visited))
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		dfs_ast(node->right, sh);
		dup2(or_stdin, STDIN_FILENO);
	}
	(close(fd[0]), close(fd[1]));
	(close(or_stdout), close(or_stdin));
}

int	cmd_node(t_ast_node *node, t_minishell *sh)
{
	char	**args;
	char	*cmd_name;
	char	*cmd_path;

	args = node->args;
	if (!args[0])
		return (null_cmd_node(node, sh));
	if (set_redirections(args, node->redirect, sh) == 1)
		return (1);
	if (is_directory(args))
		return (1);
	if (is_builtin(args[0]))
		return (exec_builtin(args, sh));
	cmd_name = ft_strdup(args[0]);
	cmd_path = find_path(cmd_name, sh);
	if (!cmd_path)
	{
		free(cmd_name);
		return (1);
	}
	if (args[0])
		free(args[0]);
	args[0] = cmd_path;
	return (exec_external(cmd_name, args, sh));
}
