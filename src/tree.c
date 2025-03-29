/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:56:19 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/27 18:33:46 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>

// => true && cat file | grep "bonjour"

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

t_ast_node	*create_example_ast(void)
{
	t_ast_node	*node;
	t_ast_node	*head;
	char **ar1; 
	char **ar2; 
	char **ar3;

	ar1 = malloc(2 * sizeof(char *));
	ar2 = malloc(3 * sizeof(char *)); 
	ar3 = malloc(3 * sizeof(char *));
	if (!ar1 || !ar2 || !ar3)
	return NULL; 

	ar1[0] = ft_strdup("true");
	ar1[1] = NULL;

	ar2[0] = ft_strdup("cat");
	ar2[1] = ft_strdup("file");
	ar2[2] = NULL;

	ar3[0] = ft_strdup("grep");
	ar3[1] = ft_strdup("bonjour");
	ar3[2] = NULL;
	node = create_ast_node(AND, NULL, NULL);
	head = node;
	node->left = create_ast_node(CMD, ar1, NULL);
	node->right = create_ast_node(PIPE, NULL, NULL);
	node = node->right;
	node->left = create_ast_node(CMD, ar2, NULL);
	node->right = create_ast_node(CMD, ar3, NULL);
	return (head);
}

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

void	dfs_ast(t_ast_node *node, void (*f)(t_ast_node *))
{
	node->visited = 1;
	f(node);
	if (node->left && !(node->left->visited))
		dfs_ast(node->left, f);
	if (node->right && !(node->right->visited))
		dfs_ast(node->right, f);
}

// I need a special data structure that stores the state of v
/*
procedure DFS(node) is
    if node is null then
        return
    
    label node as visited
    
    // Process "adjacent edges" (child edges in a binary tree)
    if node.left is not null and not visited then
        DFS(node.left)
    
    if node.right is not null and not visited then
        DFS(node.right)
*/



/*
So I need to implement, given a certain input, the tree used to categorize tokens
and execute it.
My job is not to code the parser. I just need to understand the rules and construct
true trees.

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


*/
