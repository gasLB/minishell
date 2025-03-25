/* ************************************************************************** */ /*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:32:13 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/20 17:55:26 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

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

	if (tk->value[0] != '~')
		return (0);
	if (tk->value[1] && tk->value[1] != '/')
		return (0);
	i = 1;
	while (tk->value[i])
		i++;
	*res = append_str(*res, ft_strdup(ft_getenv("HOME", env)));
	if (i > 1)
		*res = append_str(*res, ft_substr(tk->value, 1, i + 1));
	return (i);
}

char	**expanded_list(int ac, t_token **tk_list)
{
	char	**exp_list;
	int	i;

	i = 0;
	exp_list = malloc((ac - 1) * sizeof(char *));
	if (!exp_list)
		return (NULL);
	while (i < ac - 1)
	{
		exp_list[i] = tk_list[i]->expanded_value;	
		i++;
	}
	return (exp_list);


}
