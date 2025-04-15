/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:38:41 by gfontagn          #+#    #+#             */
/*   Updated: 2025/04/08 18:28:41 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>

t_redirect	*create_redirect(int n_type, char *str)
{
	t_redirect	*res;

	res = malloc(sizeof(t_redirect));
	if (!res)
		return (NULL);
	if (n_type == IN || n_type == HD)
	{
		res->in_type = n_type;
		res->out_type = -1;
		res->in_str = ft_strdup(str);
		res->out_str = NULL;
	}
	else
	{
		res->out_type = n_type;
		res->in_type = -1;
		res->out_str = ft_strdup(str);
		res->in_str = NULL;
	}
	return (res);
}

t_ast_node	*create_example_ast(void)
{
	t_ast_node	*node;
	t_ast_node	*head;
	char **ar1; 
	char **ar2; 
	char **ar3;

	ar1 = malloc(3 * sizeof(char *));
	ar2 = malloc(2 * sizeof(char *)); 
	ar3 = malloc(3 * sizeof(char *));
	if (!ar1 || !ar2 || !ar3)
		return NULL; 

	ar1[0] = ft_strdup("wc");
	ar1[1] = ft_strdup("-l");
	ar1[2] = NULL;

	ar2[0] = ft_strdup("cat");
	ar2[1] = NULL;

	ar3[0] = ft_strdup("cat");
	ar3[1] = NULL;

	node = create_ast_node(PIPE, NULL, NULL);
	head = node;
	node->left = create_ast_node(CMD, ar1, create_redirect(HD, "LIM"));
	node->right = create_ast_node(PIPE, NULL, NULL);
	node = node->right;
	node->left = create_ast_node(CMD, ar2, NULL);
	node->right = create_ast_node(CMD, ar3, create_redirect(TRUNC, "file2"));
	return (head);
}

t_minishell	*init_shell(t_env_list *env_list, t_ast_node *ast)
{
	t_minishell	*sh;

	sh = malloc(sizeof(t_minishell));
	if (!sh)
		return (NULL);
	sh->env_list = env_list;
	sh->ast = ast;
	sh->last_exit = 0;
	sh->pipe_count = 0;
	sh->pipe_fds = NULL;
	return (sh);
}

void	print_tokens(t_token **tk_list)
{
	int	i;

	i = 0;
	while (tk_list[i])
	{
		ft_printf("--- %d ---\n", i);
		ft_printf("value : %s\n", tk_list[i]->value);
		ft_printf("type : %d\n", tk_list[i]->type);
		ft_printf("quote mask : %s\n", tk_list[i]->quote_mask);
		ft_printf("exp value : %s\n", tk_list[i]->expanded_value);
		ft_printf("---------\n");
		i++;
	}
}

int	main(int ac, char **av, char **env)
{
	t_env_list	*env_list;
	t_minishell	*sh;
	t_ast_node	*ast;

	(void)ac;
	(void)av;
	ast = create_example_ast();
	env_list = populate_env(env);
	sh = init_shell(env_list, ast);
	dfs_ast(ast, sh);
	free_all_struct(sh, NULL, NULL);
	return (0);
}

// Raw input → Tokenization → Expansion → Execution
//
// TODO:
// [X] test every builtins
// [X] test with multiple pipes
// [X] handle redirections
// [X] implement here_doc 
// [ ] handle error messages
// [ ] test with quotes and double quotes
// [ ] test with complex ast
// [ ] free all memory
