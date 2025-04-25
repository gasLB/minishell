/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:50:10 by gfontagn          #+#    #+#             */
/*   Updated: 2025/04/08 17:47:03 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>

void	init_quote_n_value(t_token *token, char *q_mask, char *val, char *str)
{
	int	i;
	int	j;
	char	c;
	char	new;

	i = 0;
	j = 0;
	c = 'N';
	while (str[i])
	{
		new = set_quote_character(c, str[i]);
		if (new != c)
			c = new;
		else
		{
			q_mask[j] = c;
			val[j] = str[i];
			j++;
		}
		i++;
	}
	token->value = val;
	token->quote_mask = q_mask;
}

t_token	*init_token(char *str)
{
	t_token	*token;
	char	*val;
	char	*q_mask;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = NULL;
	token->type = -1;
	token->expanded_value = NULL;
	token->quote_mask = NULL;
	q_mask = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!q_mask)
		return (NULL);
	val = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!val)
		return (NULL);
	init_quote_n_value(token, q_mask, val, str);
	free(str);
	return (token);
}
