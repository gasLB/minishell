/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seetwoo <waltibee@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 10:48:11 by seetwoo           #+#    #+#             */
/*   Updated: 2025/06/16 14:33:34 by seetwoo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//better frees later

size_t	tok_nbr(t_token **toks)
{
	size_t	i;

	i = 0;
	while (toks[i])
		i++;
	return (i);
}

t_token	*shell_command_tok(char *self_path, size_t path_size)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(self_path);
	new->type = CMD;

	new->quote_mask = malloc(sizeof(char) * (path_size + 1));
	ft_memset(new->quote_mask, 'S', path_size);
	new->quote_mask[path_size] = '\0';

	new->transition_mask = malloc(sizeof(char) * (path_size + 1));
	ft_memset(new->transition_mask, 's', path_size);
	new->transition_mask[path_size] = '\0';
	return (new);
}

int	insert_shell_tok(t_minishell *sh, int i)
{
	t_token	**new_toks;
	size_t	sz;

	new_toks = malloc(sizeof(t_token *) * (tok_nbr(sh->token_list) + 2));
	if (!new_toks)
		return (1);
	sz = tok_nbr(&sh->token_list[i]);
	ft_memcpy(new_toks, sh->token_list, sizeof(t_token *) * i);
	new_toks[i] = shell_command_tok(sh->self_path, ft_strlen(sh->self_path));
	ft_memcpy(&(new_toks)[i + 1], &(sh->token_list)[i], sizeof(t_token *) * (sz + 1)); 
	free(sh->token_list);
	sh->token_list = new_toks;
	return (0);
}

int	add_shell_command(t_minishell *sh)
{
	int	i;

	i = 0;
	while (sh->token_list[i])
	{
		if (sh->token_list[i]->quote_mask[0] == 'P')
		{
			insert_shell_tok(sh, i);
			i++;
		}
		i++;
	}
	return (0);
}

int	subshell(t_minishell *sh, char *line)
{
	sh->line = NULL;
	sh->line_arg = line;
	if (only_space(&(sh->line_arg)))
		return (0);
	sh->token_list = init_token_list(sh->line_arg);
	add_shell_command(sh);
	set_each_token_type(&(sh->token_list));
	globbing(sh);
	if (check_syntax(sh->token_list, sh) != 0)
	{
		free_in_loop(sh);
		return (1);
	}
	sh->ast = create_ast(sh->token_list);
	dfs_ast(sh->ast, sh);
	wait_all_pids(sh);
	free_struct(sh);
	return (0);
}
