/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:56:19 by gfontagn          #+#    #+#             */
/*   Updated: 2025/04/17 22:07:50 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>

t_ast_node	*create_ast_node(int type, char **args, t_redirect *redirect)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = args;
	node->redirect = redirect;
	node->left = NULL;
	node->right = NULL;
	node->visited = 0;
	return (node);
}



/*

So I have the parsing function:
I have a full token list with values, their type, and expanded in a valid line

PRECEDENCE CLIMBING

The Core Idea

Redirections (>, <, >>, <<) - highest precedence, binds tightly to commands
Pipes (|) - medium precedence
Logical operators (&&, ||) - lowest precedence


So I need to implement, given a certain input, the tree used to categorize tokens
and execute it.

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
