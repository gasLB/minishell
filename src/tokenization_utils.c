/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 21:04:50 by gfontagn          #+#    #+#             */
/*   Updated: 2025/06/16 13:46:59 by seetwoo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_lstlen(t_token **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

t_token	**append_to_lst(t_token **l, t_token *new_s)
{
	t_token	**new_l;
	int		i;
	int		size;

	size = token_lstlen(l);
	new_l = malloc((size + 2) * sizeof(t_token *));
	if (!new_l)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_l[i] = l[i];
		i++;
	}
	new_l[i] = new_s;
	new_l[i + 1] = NULL;
	free(l);
	return (new_l);
}

t_token	**init_list(void)
{
	t_token	**res;

	res = malloc(2 * sizeof(t_token *));
	if (!res)
		return (NULL);
	res[0] = NULL;
	res[1] = NULL;
	return (res);
}

void	fill_subshell_tok(t_token *token, char *str)
{
	int	i;

	i = 1;
	while (str[i + 1])
	{
		token->value[i - 1] = str[i];
		token->quote_mask[i - 1] = 'P';
		token->transition_mask[i - 1] = 'd';
		i++;
	}
	token->value[i - 1] = '\0';
	token->quote_mask[i - 1] = '\0';
	token->transition_mask[i - 1] = '\0';
	//printf("value is %s\n", token->value);
}
