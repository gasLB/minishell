/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:38:41 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/18 19:18:29 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_env_list	*env_list;
	t_token		**token_list;
	char	*str;

	token_list = populate_tokens(ac, av);
	env_list = populate_env(env);
	return (0);
}

// Raw input → Tokenization → Expansion → Execution
//
// TODO:
// [ ] Rewrite expansion by taking into account quote_mask and wildcards
// [ ] Divide set_q_mask into multiple functions
