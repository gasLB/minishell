/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:53:55 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/28 14:04:26 by seetwoo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_in(char **args, char *filename, int in_status, t_minishell *sh)
{
	int		infile;
	int		saved_er;
	char	*cm_name;

	if (in_status == HD)
		return (here_doc(filename, sh, sh->env_list));
	if (args[0])
		cm_name = ft_strdup(args[0]);
	else
		cm_name = ft_strdup("minishell");
	infile = -1;
	if (ft_strchr(filename, '*') != NULL)
		ft_dprintf(2, "%s: %s: ambiguous redirect", cm_name, filename);
	else if (access(filename, F_OK) != 0)
		ft_dprintf(2, "%s: %s: " NO_FILE, cm_name, filename);
	else
	{
		infile = open(filename, O_RDONLY, 0644);
		saved_er = errno;
		if (infile == -1 && saved_er == EACCES)
			ft_dprintf(2, "%s: %s: " PERMISSION, cm_name, filename);
		else if (infile == -1 && saved_er == EMFILE)
			ft_dprintf(2, "error redirect: " NO_FDS);
	}
	return (free(cm_name), infile);
}

int	open_out(char **args, char *filename, mode_t mode)
{
	int		outfile;
	int		saved_er;
	char	*cm_name;

	if (args[0])
		cm_name = ft_strdup(args[0]);
	else
		cm_name = ft_strdup("minishell");
	outfile = -1;
	if (ft_strchr(filename, '*') != NULL)
		ft_dprintf(2, "%s: %s: ambiguous redirect", cm_name, filename);
	else if (access(filename, F_OK) == 0 && access(filename, W_OK) != 0)
		ft_dprintf(2, "%s: %s: " PERMISSION, cm_name, filename);
	outfile = open(filename, O_WRONLY | mode | O_CREAT, 0644);
	saved_er = errno;
	if (outfile == -1 && saved_er == EMFILE)
		ft_dprintf(2, "error redirect: " NO_FDS);
	free(cm_name);
	return (outfile);
}

int	duplicate_redir(char **args, int *file, t_redir_node *curr, t_minishell *sh)
{
	mode_t	mode;

	if (curr->type == IN || curr->type == HD)
	{
		*file = open_in(args, curr->str, curr->type, sh);
		if (*file == -1)
			return (1);
		if (dup2(*file, STDIN_FILENO) == -1)
			return (ft_dprintf(2, "error redirect: " NO_FDS));
	}
	else if (curr->type == TRUNC || curr->type == APPEND)
	{
		mode = O_APPEND;
		if (curr->type == TRUNC)
			mode = O_TRUNC;
		*file = open_out(args, curr->str, mode);
		if (*file == -1)
			return (1);
		if (dup2(*file, STDOUT_FILENO) == -1)
			return (ft_dprintf(2, "error redirect: " NO_FDS));
	}
	return (0);
}

int	set_redirections(char **args, t_redir_node *redir, t_minishell *sh)
{
	int				file;
	t_redir_node	*curr;

	if (!redir || !is_redirect(redir->type))
		return (0);
	curr = redir;
	file = -1;
	while (curr)
	{
		if (duplicate_redir(args, &file, curr, sh) != 0)
		{
			if (file != -1)
				close(file);
			return (1);
		}
		curr = curr->next;
		close(file);
	}
	return (0);
}

void	reset_redirections(t_redir_node *redir, t_minishell *sh)
{
	if (!redir || !is_redirect(redir->type))
		return ;
	if (dup2(sh->original_stdin, STDIN_FILENO) == -1)
		ft_dprintf(2, "error redirect: " NO_FDS);
	if (dup2(sh->original_stdout, STDOUT_FILENO) == -1)
		ft_dprintf(2, "error redirect: " NO_FDS);
}
