/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 19:41:21 by gfontagn          #+#    #+#             */
/*   Updated: 2025/04/18 21:42:46 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>

t_redirect	*init_redirection(void)
{
	t_redirect	*res;

	res = malloc(sizeof(t_redirect));
	if (!res)
		return (NULL);
	res->in = NULL;
	res->out = NULL;
	return (res);
}

t_red_node	*init_red_node(int type, char *str)
{
	t_red_node	*node;

	node = malloc(sizeof(t_red_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->str = str;
	node->next = NULL;
	return (node);
}

t_red_node	*add_red_node(t_red_node **head, int type, char *str)
{
	t_red_node	*new_node;
	t_red_node	*curr;

	new_node = init_red_node(type, str);
	if (!new_node)
		return (NULL);
	if (!(*head))
	{
		*head = new_node;
		return (new_node);
	}
	curr = *head;
	while (curr->next)
		curr = curr->next;
	curr->next = new_node;
	return (new_node);
}

t_redirect	*set_one_redir(t_redirect *red, t_token ***tkp)
{
	int	type;
	char	*file;

	if (!red)
		red = init_redirection();
	if (!red || !(*tkp))
		return (NULL);
	type = (**tkp)->type;
	(*tkp)++;
	if (!(*tkp) || !(**tkp) || !is_file((**tkp)->type))
		return (red);
	file = ft_strdup((**tkp)->expanded_value);
	if (!file)
		return (red);
	if (type == IN || type == HD)
	{
		if (!add_red_node(&(red->in), type, file))
			(free(file), return (red));	// why free file ? -> if the call fails
	}
	else
	{
		if (!add_red_node(&(red->out), type, file))
			(free(file), return (red));
	}
	return (red);
}

/*




Actually we can have mulitple redirections in line

OUT:

only the last is taken to account but the previous ones create the files
Actally no, the middle file are emptied for TRUNC

IN:

same shit

EXAMPLES

cat > b > c < test

SOLUTION

Maybe t_redirect would have a in_redirect and out_redirect list
It would first traverse the input list to retreive the value and set the STDIN
Then traverse the output list to create the files and empty them for all TRUNC 


*/
