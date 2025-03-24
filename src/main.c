/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:38:41 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/22 19:59:32 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>

t_minishell	*init_shell(t_env_list *env_list)
{
	t_minishell	*sh;

	sh = malloc(sizeof(sh));
	if (!sh)
		return (NULL);
	sh->env_list = env_list;
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
	t_token		**token_list;
	t_minishell	*sh;
	char	**arg_list;

	env_list = populate_env(env);
	sh = init_shell(env_list);
	token_list = expand_tokens(populate_tokens(ac, av), sh, env_list);
	arg_list = expanded_list(ac, token_list);	
	ft_unset(ac - 1, arg_list, env_list);
	ft_env(env_list);
	return (0);
}

// Raw input → Tokenization → Expansion → Execution
//
// TODO:
// [X] Divide set_q_mask into multiple functions
// [X] test export 
// [X] implement a function to put all expanded values inside char **
// [X] make sure 'echo' takes into account env
// [X] test unset
// [ ] implement other builtins
// [ ] correct leaks inside all current code (ex: ft_strdup, etc...)
// [ ] implement input system with readline
