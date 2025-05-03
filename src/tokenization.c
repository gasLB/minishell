/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:50:10 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/01 17:20:28 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>

char	get_quote_character(char c, char new, int i, int *last)
{
	if ((c == 'D' && new == '"') || (c == 'S' && new == '\''))
	{
		if (last)
			*last =  i;
		return ('N');
	}
	else if (c == 'N' && new == '"')
		return ('D');
	else if (c == 'N' && new == '\'')
		return ('S');
	return (c);
}

int	check_end(char *str, int i, int last)
{
	if (i > 0 && last == i - 1)
	{
		if (str[i - 1] == '"' && str[i] == '"')
			return (1);
		if (str[i - 1] == '\'' && str[i] == '\'')
			return (1);
	}
	return (0);
}

void	init_quote_n_value(char *val, char *q_mask, char *str, int last_end)
{
	int		i;
	int		j;
	char	c;
	char	new;

	i = 0;
	j = 0;
	c = 'N';
	while (str[i])
	{
		new = get_quote_character(c, str[i], i, &last_end);
		if (new != c && !check_end(str, i, last_end))
			c = new;
		else
		{
			if (check_end(str, i, last_end))
				q_mask[j] = 'T';
			else
			{
				q_mask[j] = c;
				val[j++] = str[i];
			}
		}
		i++;
	}
}

t_token	*init_token(char *str)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = -1;
	token->expanded_value = NULL;
	token->quote_mask = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!token->quote_mask)
		return (NULL);
	token->value = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!token->value)
		return (NULL);
	init_quote_n_value(token->value, token->quote_mask, str, -1);
	free(str);
	return (token);
}
// I will rebuild this by splitting it into 2 functions
