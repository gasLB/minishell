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

void	free_all_struct(t_minishell *sh, t_token **tk_list, char **arg_list)
{
	free_env_list(sh->env_list);
	free(sh);	
	free_token_list(tk_list);
	free(arg_list);
}
