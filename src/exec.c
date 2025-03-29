/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 19:12:12 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/29 19:51:00 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>

/*
Ok now we must think about execution.

So what is the rule like?

We go through the tree following the DFS algo.

1. OPERATORS: AND / OR
- cmd1 && cmd2: run cmd2 if cmd1 succeeds (l_e_s == 0)
- cmd1 || cmd2: run cmd2 if cmd1 fails (l_e_s != 0)

How to implement that into the DFS?
=> instead of using l_e_s, we could have a var node->exit_status
for every command
But how to back-propagate up to the pipe for example?
Don't forget that the command is linear. I think we could safely use l_e_s
! All of this apply only for the right. Left is always ran

2. PIPES
create a pipe each time we encounter a node pipe
Then do like pipex and take into account redirect of commands
* */
