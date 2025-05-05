/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_traverse_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:33:41 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/05 15:17:32 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

int	null_cmd_node(t_ast_node *node, t_minishell *sh)
{
	if (!node || !node->redirect)
		return (1);
	return (set_redirections(node->args, node->redirect, sh));
}

int	is_directory(char *name)
{
	DIR	*dir;

	dir = opendir(name);
	if (dir)
	{
		closedir(dir);
		printf_fd(2, "minishell: %s: Is a directory\n", name);
		return (1);
	}
	return (0);
}
// directory permissions are not taken into account
