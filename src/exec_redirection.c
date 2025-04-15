/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:53:55 by gfontagn          #+#    #+#             */
/*   Updated: 2025/04/15 17:46:33 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int	open_in(char **args, char *file_name, int in_status, t_minishell *sh)
{
	int	infile;

	infile = -1;
	if (in_status == HD)
		return (here_doc(file_name, args, sh));
	if (access(file_name, F_OK) != 0)
		ft_printf("%s: %s: No such file or directory\n", args[0], file_name);
	else
	{
		infile = open(file_name, O_RDONLY, 0644);
		if (infile == -1)
			ft_printf("%s: %s: Permission denied\n", args[0], file_name);
	}
	return (infile);
}

int	open_out(char **args, char *file_name, int in_status)
{
	int	outfile;

	outfile = -1;
	if (access(file_name, F_OK) == 0 && access(file_name, W_OK) != 0)
	{
		ft_printf("%s: %s: Permission denied\n", args[0], file_name);
		return (-1);
	}
	if (in_status == TRUNC)
		outfile = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if (in_status == APPEND)
		outfile = open(file_name, O_WRONLY | O_APPEND | O_CREAT, 0644);
	return (outfile);
}

int	set_redirections(char **args, t_ast_node *node, t_minishell *sh)
{
	int	in_status;
	int	out_status;
	int	infile;
	int	outfile;

	if (node->redirect == NULL)
		return (0);
	in_status = node->redirect->in_type;
	out_status = node->redirect->out_type;
	if (in_status != -1)
	{
		infile = open_in(args, node->redirect->in_str, in_status, sh);
		if (infile == -1 || dup2(infile, STDIN_FILENO) == -1)
			return (1);
		close(infile);
	}
	if (out_status != -1)
	{
		outfile = open_out(args, node->redirect->out_str, out_status);
		if (outfile == -1 || dup2(outfile, STDOUT_FILENO) == -1)
			return (1);
		close(outfile);
	}
	return (0);
}
// ! openend outfile and infile need to be closed right?
