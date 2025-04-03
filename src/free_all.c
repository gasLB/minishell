/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:28:44 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/25 14:25:43 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>

/*
What is to be freed ?
-> the shell element
-> env_list + all its nodes + keys (+ values)
-> token_list + all tokens + values, q_masks, expanded_values
-> arg_list
*/

void	free_env_list(t_env_list *env_list)
{
	t_env_node	*cur;
	t_env_node	*next;

	cur = env_list->head;
	while (cur)
	{
		next = cur->next;
		free(cur->key);
		if (cur->value)
			free(cur->value);
		free(cur);
		cur = next;
	}
	free(env_list);
}

void	free_token_list(t_token **tk_list)
{
	int	i;

	i = 0;
	while (tk_list[i])
	{
		free(tk_list[i]->value);
		free(tk_list[i]->expanded_value);
		free(tk_list[i]->quote_mask);
		free(tk_list[i]);
		i++;
	}
	free(tk_list);
}

void	free_ast(t_ast_node *ast)
{
	(void)ast;
	ft_printf("freeing ast...\n");
}

void	free_str_list(char **lst)
{
	int	i;

	if (!lst)
		return;
	i = 0;
	while (lst[i])
		free(lst[i]);
	free(lst);
}

void	free_all_struct(t_minishell *sh, char **arg_list, char **envp)
{
	free_env_list(sh->env_list);
	free_ast(sh->ast);
	free(sh);	
	free_str_list(arg_list);
	free_str_list(envp);
}
