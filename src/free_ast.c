/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 16:21:52 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/01 16:25:27 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

void	free_redirect(t_redir_node *red)
{
	t_redir_node	*n;
	t_redir_node	*curr;

	if (!red)
		return ;
	curr = red;
	while (curr)
	{
		n = curr->next;
		if (curr->str)
			free(curr->str);
		free(curr);
		curr = n;
	}
}

void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->visited == 2)
		return ;
	node->visited = 2;
	if (node->left && node->left->visited != 2)
		free_ast(node->left);
	if (node->right && node->right->visited != 2)
		free_ast(node->right);
	if (node->args)
		free_str_list(node->args);
	if (node->redirect)
		free_redirect(node->redirect);
	if (node)
		free(node);
}
