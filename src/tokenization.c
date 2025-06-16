/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:50:10 by gfontagn          #+#    #+#             */
/*   Updated: 2025/06/16 13:02:07 by seetwoo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	get_quote_character(char c, char new, int i, int *last)
{
	if ((c == 'D' && new == '"') || (c == 'S' && new == '\'')
		|| (c == 'P' && new == ')'))
	{
		if (last)
			*last = i;
		return ('N');
	}
	else if (c == 'N' && new == '"')
		return ('D');
	else if (c == 'N' && new == '\'')
		return ('S');
	else if (c == 'N' && new == '(')
		return ('P');
	return (c);
}

char	get_transition(char *str, int i, int last)
{
	if (i > 1 && last == i - 2)
	{
		if (str[last] == '"' && str[i - 1] == '"')
			return ('d');
		if (str[last] == '\'' && str[i - 1] == '\'')
			return ('s');
	}
	if (i > 0 && last == i - 1)
	{
		if (str[last] == '"' && str[i] != '"')
			return ('d');
		if (str[last] == '\'' && str[i] != '\'')
			return ('s');
	}
	return ('n');
}

void	init_quote_n_value(t_token *token, char *str)
{
	t_idx	i;

	i.last_end = -1;
	i.i = 0;
	i.j = 0;
	i.c = 'N';
	if (str[0] == '(')
		return (fill_subshell_tok(token, str));
	while (str[i.i])
	{
		i.new = get_quote_character(i.c, str[i.i], i.i, &(i.last_end));
		if (i.new != i.c)
			i.c = i.new;
		else
		{
			token->quote_mask[i.j] = i.c;
			token->value[i.j] = str[i.i];
			token->transition_mask[i.j] = get_transition(str, i.i, i.last_end);
			i.j++;
		}
		i.i++;
	}
}

t_token	*init_token(char *str)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (free_token(token));
	token->type = -1;
	token->quote_mask = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!token->quote_mask)
		return (free_token(token));
	token->value = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!token->value)
		return (free_token(token));
	token->transition_mask = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!token->transition_mask)
		return (free_token(token));
	init_quote_n_value(token, str);
	free(str);
	return (token);
}
