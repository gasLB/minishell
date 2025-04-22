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

t_redir_node	*init_redir_node(int type, char *str)
{
	t_redir_node	*node;

	node = malloc(sizeof(t_redir_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->str = str;
	node->next = NULL;
	return (node);
}

t_redir_node	*add_redir_node(t_redir_node **head, int type, char *str)
{
	t_redir_node	*new_node;
	t_redir_node	*curr;

	new_node = init_redir_node(type, str);
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

t_redir_node	*set_one_redir(t_redir_node *redir, t_token ***tkp)
{
	int	type;
	char	*file;

	if (!redir && !(*tkp))
		return (NULL);
	type = (**tkp)->type;
	if (!redir)
		redir = init_redir_node(type, (**tkp)->expanded_value);
	(*tkp)++;
	if (!(*tkp) || !(**tkp) || !is_file((**tkp)->type))
		return (redir);
	file = ft_strdup((**tkp)->expanded_value);
	if (!file)
		return (redir);
	if (!add_redir_node(&redir, type, file))
		free(file);
	return (redir);
}

/*



SOLUTION

Maybe t_redirect would have a in_redirect and out_redirect list
It would first traverse the input list to retreive the value and set the STDIN
Then traverse the output list to create the files and empty them for all TRUNC 


*/
