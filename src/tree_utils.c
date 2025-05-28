/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:24:23 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/28 13:55:43 by seetwoo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*init_ast_node(void)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = -1;
	node->args = NULL;
	node->redirect = NULL;
	node->left = NULL;
	node->right = NULL;
	node->visited = 0;
	return (node);
}

t_ast_node	*set_ast_node(int type, char **args, t_redir_node *red)
{
	t_ast_node	*node;

	node = init_ast_node();
	if (!node)
		return (NULL);
	node->args = args;
	node->type = type;
	node->redirect = red;
	return (node);
}

int	get_precedence(int type)
{
	if (type == -1)
		return (-1);
	if (is_operator(type))
		return (1);
	if (is_pipe(type))
		return (2);
	return (3);
}

int	is_op_or_pipe(int type)
{
	return (is_operator(type) || is_pipe(type));
}
