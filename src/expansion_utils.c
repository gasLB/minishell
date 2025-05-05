/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:55:46 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/05 19:09:52 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

int	translation(char **res, t_token *tk, int i)
{
	int	begin;

	begin = i;
	while (tk->quote_mask[i] == 'S' || tk->quote_mask[i] == 'D')
		i++;
	*res = append_str(*res, ft_substr(tk->value, begin, i - 1));
	return (i);
}

char	*init_str(void)
{
	char	*res;

	res = malloc(sizeof(char));
	if (!res)
		return (NULL);
	res[0] = '\0';
	return (res);
}

int	handle_tilde(char **res, t_token *tk, t_env_list *env)
{
	int	i;
	char	*home;

	if (tk->value[0] != '~')
		return (0);
	if (tk->value[1] && tk->value[1] != '/')
		return (0);
	i = 1;
	while (tk->value[i])
		i++;
	home = ft_getenv("HOME", env);
	if (!home)
		return (0);
	*res = append_str(*res, ft_strdup(home));
	if (i > 1)
		*res = append_str(*res, ft_substr(tk->value, 1, i + 1));
	return (i);
}

char	**expanded_list(int ac, t_token **tk_list)
{
	char	**exp_list;
	int		i;

	i = 0;
	exp_list = malloc((ac + 1) * sizeof(char *));
	if (!exp_list)
		return (NULL);
	while (i < ac)
	{
		exp_list[i] = ft_strdup(tk_list[i]->expanded_value);
		i++;
	}
	exp_list[i] = NULL;
	return (exp_list);
}
