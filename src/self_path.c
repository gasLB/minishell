/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   self_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seetwoo <waltibee@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 13:10:27 by seetwoo           #+#    #+#             */
/*   Updated: 2025/06/15 13:35:50 by seetwoo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_self_path(t_minishell *sh)
{
	size_t	len;

	len = readlink("/proc/self/exe", sh->self_path, 256 - 1);
	sh->self_path[len] = '\0';
	//printf("self path is : %s\n", sh->self_path);
}
