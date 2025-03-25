/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:18:06 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/25 13:24:55 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

/*
The idea is to use readline to have a command-input environment
How to use it without the parsing part ?
Well for now we just need to isolate each word and put it in a large array of tokens

I'm not sure it's really useful...
*/

void	read_print_input(void)
{
	char *rl;

	rl = readline("Minishell > ");
	if (rl)
		add_history(rl);
	while (*rl)
	{
		rl = readline("Minishell > ");
		add_history(rl);
	}
	printf("%s\n", rl);
	free(rl);	
}

