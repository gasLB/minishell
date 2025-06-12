/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:16:10 by gfontagn          #+#    #+#             */
/*   Updated: 2025/06/12 16:21:05 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_status(int status, int *sigint, t_minishell *sh)
{
	int	sig;

	sig = 0;
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			*sigint = 1;
		else if (sig == SIGQUIT)
			ft_printf("Quit (core dumped)\n");
		if (sh->last_command_type == EXTERNAL)
			sh->last_exit = 128 + sig;
	}
}

void	wait_all_pids(t_minishell *sh)
{
	int	status;
	int	i;
	int	sigint_received;

	if (sh->pid_count == 0)
		return ;
	sigint_received = 0;
	i = 0;
	status = 0;
	while (i < sh->pid_count)
	{
		waitpid(sh->pids[i], &status, 0);
		check_status(status, &sigint_received, sh);
		i++;
	}
	if (sigint_received)
		printf_fd(2, "\n");
	if (WIFEXITED(status) && sh->last_command_type != BUILTIN)
		sh->last_exit = WEXITSTATUS(status);
}
