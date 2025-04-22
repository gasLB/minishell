/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:56:19 by gfontagn          #+#    #+#             */
/*   Updated: 2025/04/18 21:44:32 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>

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

int	get_precedence(t_token *token)
{
	if (!token)
		return (-1);
	if (is_operator(token->type))
		return (1);
	if (is_pipe(token->type))
		return (2);
	return (3);
}

// we can have a null-command with only redirections in parse_command
// args is initalized with init_list but args[0] = NULL
t_ast_node	*parse_command(t_token ***tk_list_pt)
{
	char	**args;
	char	*value;
	t_redir_node	*red;
	int	type;

	red = NULL;
	args = init_list();
	if (!args)
		return (NULL);
	while (*tk_list_pt && get_precedence(**tk_list_pt) == 3)
	{
		value = (**tk_list_pt)->expanded_value;
		type = (**tk_list_pt)->type;
		if (is_command(type))
			args[0] = ft_strdup(value);
		else if (type == ARG)
			args = append_to_lst(args, ft_strdup(value));
		else if (is_redirect(type))
		{
			red = set_one_redir(red, tk_list_pt);
			continue;
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
	t_token		*lookahead;
	t_ast_node	*right;

	while (op_greater_precedence(**tkp, min_prec))
	{
		operator = (**tkp)++;
		right = parse_command(tkp);
		while (op_greater_precedence(**tkp, get_precedence(operator)))
		{
			lookahead = (**tkp)++;
			right = parse_expr(right, get_precedence(lookahead), tkp);
		}
		left = combine_nodes(left, right, operator);
	}
	return (left);
}

t_ast_node	*create_ast(t_token **tk_list)
{
	t_ast_node	*head;

	if (!tk_list || !tk_list[0])
		return (NULL);
	head = parse_command(&tk_list);
	if (!head)
		return (NULL);
	return (parse_expr(head, 0, &tk_list));
}

/*

So I have the parsing function:
I have a full token list with values, their type, and expanded in a valid line

PRECEDENCE CLIMBING

# Intro

How many precedence levels do I have?
-> 3

Redirections (>, <, >>, <<) - highest precedence, binds tightly to commands
Pipes (|) - medium precedence
Logical operators (&&, ||) - lowest precedence

An infix expression is a sequence of primary expressions separated by operators.
primary expr is one at the highest precedence level

--- Pseudo-code

parse_expression ()
    return parse_expression_1 (parse_primary (), 0)

parse_expression_1 (lhs, min_precedence)
    while the next token is a binary operator whose precedence is >= min_precedence
        op := next token
	consume_token() // advance to the next token
        rhs := parse_primary ()
        while the next token is a binary operator whose precedence is greater
                 than op's
            lookahead := next token
	    consume_token()
            rhs := parse_expression_1 (rhs, lookahead's precedence)
        lhs := the result of applying op with operands lhs and rhs
    return lhs


--- Explanation

1.The first call parses the leftmost primary expression (a command)
2. Passes it to parse_expression_1 with a min precedence of 0

parse_expression_1:
3. continue until we see operator with precedence >= minimum 
4. When we see operator with precedence >= minimum, parse primary for its right side


--- Example: 2 + 3 * 4 + 5

- parse primary 2 as lhs
- calls parse_expression_1(2, 0)
- op = +
- rhs = 3
- lookahead = *
- rhs = parse_expression_1(3, 2)
	- op = *
	- rhs = 4
	- return rhs = 3 * 4 = 12
- rhs = 12
- lhs = 2 + 12
- op = +
- rhs = 5
- lhs = 14 + 5 = 19
- return 19


---
AST 1

we have AST NODES, which point to left and right.
=> we are traversing the AST recursively

ex: int execute(t_node node)
{
 if (node.type == PIPE)
   return (execute_pipe(node.left, node.right));
 else
   return (execute_simple_command(node.value))
}

-> But if node.right also contains a pipe, we must call execute again?

void	print_ast_node(t_ast_node *node)
{
	int	i;

	i = 0;
	if (node == NULL)
	{
		ft_printf("NULL node\n");
		return;
	}
	ft_printf("----- Node -----\n");
	ft_printf("type: %i\n", node->type);
	ft_printf("left: %p\n", node->left);
	ft_printf("right: %p\n", node->right);
	while (node->args && node->args[i])
	{
		ft_printf("args[%i]: %s\n", i, node->args[i]);
		i++;
	}
	ft_printf("----------------\n");
}

AST 2

each node represents a process to launch or an operator
a specific function is called for each type of nodes

- operators: &&, ||, |, <, >, >>.
- commands: external, builtins.

order of priority:
1. &&, ||
2. | 

WHAT IS PRECEDENCE CLIMBING? 

2 + 3 * 4 * 5 - 6

|---------------|   : prec 1
    |-------|       : prec 

2 + 3 ^ 2 * 3 + 4

|---------------|   : prec 1
    |-------|       : prec 2
    |---|           : prec 3

Associativity:
- Left associativity:
	2 + 3 + 4 = (2 + 3) + 4
- right associativity:
	2 ^ 3 ^ 4 = 2 ^ (3 ^ 4)

Atoms are either numbers or parenthesized expressions.
Expressions are atoms connectedd by binary operators

--- NOTES

'&&' and '||' can only be used betweem commands

what should I do know? 
Wb is late and my part is completely done.
If I can do the parsing in only one afternoon, then I should do it
I'll think 10 minutes about it.
The parsing job: check the validity of the the input, then if valid construct the ast tree
ok. I will try to do it.
*/
