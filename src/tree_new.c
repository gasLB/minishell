/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_new.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:26:21 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/28 13:54:53 by seetwoo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*parse_command(t_token ***tk_list_pt, char **args)
{
	char			*value;
	t_redir_node	*red;
	int				type;

	red = NULL;
	while (args && **tk_list_pt && get_precedence((**tk_list_pt)->type) == 3)
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

int	peek_token_type(t_token *token)
{
	if (!token)
		return (-1);
	return (token->type);
}

t_ast_node	*combine_nodes(t_ast_node *left, t_ast_node *right, int op)
{
	t_ast_node	*new_node;

	new_node = init_ast_node();
	if (!new_node)
		return (NULL);
	new_node->type = op;
	new_node->left = left;
	new_node->right = right;
	return (new_node);
}

t_ast_node	*parse_expr(t_ast_node *left, int prec, t_token ***tklp)
{
	int			look;
	int			op;
	t_ast_node	*right;

	look = peek_token_type(**tklp);
	while (is_op_or_pipe(look) && get_precedence(look) >= prec)
	{
		op = look;
		(*tklp)++;
		right = parse_command(tklp, init_list());
		look = peek_token_type(**tklp);
		while (is_op_or_pipe(look) && get_precedence(look) > get_precedence(op))
		{
			right = parse_expr(right, get_precedence(op) + 1, tklp);
			look = peek_token_type(**tklp);
		}
		left = combine_nodes(left, right, op);
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
