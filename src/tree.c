/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:56:19 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/01 18:39:56 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>

t_ast_node	*parse_command(t_token ***tk_list_pt, char **args)
{
	char			*value;
	t_redir_node	*red;
	int				type;

	red = NULL;
	while (*tk_list_pt && get_precedence(**tk_list_pt) == 3)
	{
		value = (**tk_list_pt)->expanded_value;
		type = (**tk_list_pt)->type;
		if (is_command(type))
		{
			if (args[0] != NULL)
				free(args[0]);
			args[0] = ft_strdup(value);
		}
		else if (type == ARG)
			args = append_to_lst(args, ft_strdup(value));
		else if (is_redirect(type))
		{
			set_one_redir(&red, tk_list_pt);
			continue ;
		}
		(*tk_list_pt)++;
	}
	return (set_ast_node(CMD, args, red));
}

int	op_greater_precedence(t_token *token, int min_precedence)
{
	if (!token)
		return (0);
	if (is_operator(token->type) || is_pipe(token->type))
	{
		if (get_precedence(token) >= min_precedence)
			return (1);
	}
	return (0);
}

t_ast_node	*combine_nodes(t_ast_node *left, t_ast_node *right, t_token *op)
{
	t_ast_node	*new_node;

	new_node = init_ast_node();
	if (!new_node)
		return (NULL);
	new_node->type = op->type;
	new_node->left = left;
	new_node->right = right;
	return (new_node);
}

t_ast_node	*parse_expr(t_ast_node *left, int min_prec, t_token ***tkp)
{
	t_token		*operator;
	t_ast_node	*right;
	char		**args;

	while (*tkp && **tkp && op_greater_precedence(**tkp, min_prec))
	{
		operator = **tkp;
		(*tkp)++;
		args = init_list();
		if (!args)
			return (NULL);
		right = parse_command(tkp, args);
		if (!right)
			return (NULL);
		left = combine_nodes(left, right, operator);
		if (!left)
			return (NULL);
	}
	return (left);
}

t_ast_node	*create_ast(t_token **tk_list)
{
	t_ast_node	*head;
	char		**args;

	if (!tk_list || !tk_list[0])
		return (NULL);
	args = init_list();
	if (!args)
		return (NULL);
	head = parse_command(&tk_list, args);
	if (!head)
		return (NULL);
	return (parse_expr(head, 0, &tk_list));
}
