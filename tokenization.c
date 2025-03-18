/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:50:10 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/18 19:04:43 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"

char	*strdup_without_quotes(char *str)
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
		if (str[i] != '"' && str[i] ! = '\\')
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
	char	current;

	res = ft_calloc(ft_strlen(val) + 1, sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	current = 'N';
	while (str[i])	// could reduce all of this by a simple mapping or external funct
	{
		if (str[i] == '\'')
		{
			if (current == 'S')
				current = 'N';
			else if (current == 'D')
				res[j++] = 'S'; 
			else if (current == 'N')
				current = 'S';
		}
		else if (str[i] == '"')
		{
			if (current == 'D')
				current = 'N';
			else if (current == 'S')
				res[j++] = 'D';
			else if (current == 'N')
				current = 'D';	
		}
		else if (str[i] == '\\')
		{
			if (current == 'N') // ! not valid in bash if \ is last char
				res[j++] = 'B';
			else
				res[j++] = current; // ! not for some special doubles
		}
		else
			res[j++] = current;
	}
	return (res);
}

t_token	*init_token(char *str)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = strdup_without_quotes(str);
	if (!token->value)
		return (NULL);
	token->type = -1;	// to do later, maybe not at the right place
	token->expanded_value = NULL;
	token->quote_mask = set_q_mask(token->value, str);
	if (!token->mask)
		return (NULL);
	return (token);
}

t_token	**populate_token_list(int ac, char **av)
{
	t_token	**token_list;
	int	i;

	if (ac <= 1)
		return (NULL);
	token_list = malloc(ac * sizeof(t_token *));
	if (!token_list)
		return (NULL);
	i = 0;
	while (i < ac)
	{
		token_list[i] = init_token(av[i]);
		i++;
	}
	return (token_list);
}
