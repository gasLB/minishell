/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seetwoo <waltibee@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 10:48:11 by seetwoo           #+#    #+#             */
/*   Updated: 2025/06/15 12:25:12 by seetwoo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//better frees later

int	subshell(t_minishell *sh, char *line)
{
	sh->line = NULL;
	sh->line_arg = line;
	if (only_space(&(sh->line_arg)))
		return (0);
	sh->token_list = init_token_list(sh->line_arg);
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
