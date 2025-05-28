/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 21:03:27 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/28 14:05:24 by seetwoo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_pipe_fd(int fd1, int fd2, t_minishell *sh)
{
	if (sh->pipe_count + 2 >= MAX_FD)
	{
		ft_dprintf(2, "pipe error: Too many open pipes\n");
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

int	dup_pipe(t_ast_node *n, int fd[2], int or_std[2], t_minishell *sh)
{
	if (n->left && !(n->left->visited))
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			return (ft_dprintf(2, "pipe error: " NO_FDS));
		close(fd[1]);
		dfs_ast(n->left, sh);
		if (dup2(or_std[1], STDOUT_FILENO) == -1)
			return (ft_dprintf(2, "pipe error: " NO_FDS));
	}
	if (n->right && !(n->right->visited))
	{
		if (dup2(fd[0], STDIN_FILENO) == -1)
			return (ft_dprintf(2, "pipe error: " NO_FDS));
		close(fd[0]);
		dfs_ast(n->right, sh);
		if (dup2(or_std[0], STDIN_FILENO) == -1)
			return (ft_dprintf(2, "pipe error: " NO_FDS));
	}
	return (0);
}
