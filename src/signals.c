/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:58:56 by gfontagn          #+#    #+#             */
/*   Updated: 2025/04/24 15:21:14 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

void	handle_sigint(int sig)
{
	(void)sig;
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	if (g_signal_pid == 0)
		rl_redisplay();
}

void	set_signals(void)
{
	signal(SIGINT, &handle_sigint);
	signal()
	signal(SIGQUIT, SIG_IGN);
}

/*

I don't understand how signals are really handled
I should dig deepe
handled signals:

ctrl-C: displays a new prompt on a new line (^C written)
ctrl-D: exit the shell (exit written)
ctrl-\: does nothing

where to put it?
only at the initialization or during the parent process
*/
