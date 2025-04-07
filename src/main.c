/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:38:41 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/27 13:47:50 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>

t_minishell	*init_shell(t_env_list *env_list, t_ast_node *ast)
{
	t_minishell	*sh;

	sh = malloc(sizeof(t_minishell));
	if (!sh)
		return (NULL);
	sh->env_list = env_list;
	sh->ast = ast;
	sh->last_exit = 0;
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
	dfs_ast(ast, sh, handle_node);
	return (0);
}

// Raw input → Tokenization → Expansion → Execution
//
// TODO:
// [ ] make cmd exec work
// [ ] handle pipes
// [ ] handle operators
// [ ] handle redirections
// [ ] handle proper error messages and formatting: minishell vs external command name
// [ ] implement pipe function when encountering pipe node
// [ ] implement operator function for operators
// [ ] look at pipex for execution
//
// [ ] implement here_doc 
