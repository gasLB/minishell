/* ************************************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbeschon <wbeschon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:53:35 by wbeschon          #+#    #+#             */
/*   Updated: 2025/06/02 11:50:08 by seetwoo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_wild_tok(t_token **wild_toks, char *entry)
{
	t_token	*current;
	t_token	*new;

	new = init_wild_token(entry);
	if (!new)
		return (1);
	if (!(*wild_toks))
		*wild_toks = new;
	else
	{
		current = *wild_toks;
		while (current->next)
			current = current->next;
		current->next = new;
	}
	return (0);
}

int	create_wild_toks(char *wild, t_token **wild_toks)
{
	struct dirent	*entry;
	DIR				*dir;
	char			*wild_dup;

	*wild_toks = NULL;
	dir = opendir(".");
	if (!dir)
		return (printf("does not work"));
	entry = readdir(dir);
	while (entry)
	{
		wild_dup = ft_strdup(wild);
		if (are_matching(wild_dup, entry->d_name))
			add_wild_tok(wild_toks, entry->d_name);
		free(wild_dup);
		entry = readdir(dir);
	}
	return (0);
}

size_t	list_size(t_token* head)
{
	size_t	i;

	i = 0;
	while (head)
	{
		i++;
		head = head->next;
	}
	return (i);
}

int	insert_wild_toks(t_minishell *sh, int *wild, int tab_size)
{
	t_token	*wild_toks;
	t_token	**tok_array;
	t_token	**new_array;
	int		i;
	int		j;

	tok_array = sh->token_list;
	create_wild_toks(tok_array[*wild]->value, &wild_toks);
	if (!wild_toks)
	{
		(*wild)++;
		return (1);
	}
	new_array = malloc(sizeof(t_token *) * (list_size(wild_toks) + tab_size + 1));
	if (!new_array)
		return (1);											//frees later
	i = 0;
	while (i != *wild)
	{
		new_array[i] = tok_array[i];
		i++;
	}
	j = 0;
	while (wild_toks)
	{
		new_array[i + j] = wild_toks;
		wild_toks = wild_toks->next;
		j++;
	}
	new_array[i + j] = NULL;
	*wild += j;
	free(tok_array);
	sh->token_list = new_array;
	return (0);
}

int	globbing(t_minishell *sh)
{
	int		i;
	char	*ast;
	int		ast_pos;
	int		tab_size;

	i = 0;
	while (sh->token_list[i])
	{
		ast = ft_strchr(sh->token_list[i]->value, '*');
		ast_pos = ast - sh->token_list[i]->value;
		if (!ast || sh->token_list[i]->type != ARG || sh->token_list[i]->quote_mask[ast_pos] != 'N')
		{
			i++;
			continue ;
		}
		tab_size = 0;
		while (sh->token_list[tab_size])
			tab_size++;
		insert_wild_toks(sh, &i, tab_size);
	}
	return (1);
}
