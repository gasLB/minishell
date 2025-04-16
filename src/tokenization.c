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

char	*strdup_without_quotes(char *str)	//rewrite with ft_strspd
{
	char	*res;
	int	j;
	int	i;

	i = 0;
	j = 0;
	res = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] != '"' && str[i] != '\\')
			res[j++] = str[i];
		i++;
	}
	return (res);
}

char	*set_q_mask(char *val, char *str)
{
	int	i;
	int	j;
	char	*res;
	char	c;
	char	new;

	res = ft_calloc(ft_strlen(val) + 1, sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	c = 'N';
	while (str[i])
	{
		new = set_quote_character(c, str[i]));
		if (new != c)
			c = new;
		else
			res[j++] = c;
		i++;
	}
	res[j] = '\0';
	return (res);
}

t_token	*init_token(char *str)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = strdup_without_quotes(str);	// to replace
	if (!token->value)
		return (NULL);
	token->type = find_token_type(token->value, env_list)
	token->expanded_value = NULL;
	token->quote_mask = set_q_mask(token->value, str);
	if (!token->quote_mask)
		return (NULL);
	free(str);	// recently added !
	return (token);
}

t_token	**populate_tokens(int ac, char **av)
{
	t_token	**token_list;
	int	i;

	if (ac < 1)
		return (NULL);
	token_list = malloc((ac + 1) * sizeof(t_token *));
	if (!token_list)
		return (NULL);
	i = 0;
	while (i < ac)
	{
		token_list[i] = init_token(av[i]);
		i++;
	}
	token_list[i] = NULL;
	return (token_list);
}
