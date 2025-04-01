/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:24:24 by gfontagn          #+#    #+#             */
/*   Updated: 2025/04/01 17:03:17 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>

int	ft_lstlen(char **args)
{
	int	i;

	while (args[i])
		i++;
	return (i);
}

int	is_builtin(char *str)
{
	if (!str)
		return (0);
	if (ft_isequal(str, "echo"))
		return (1);
	if (ft_isequal(str, "pwd"))
		return (2);
	if (ft_isequal(str, "cd"))
		return (3);
	if (ft_isequal(str, "export"))
		return (4);
	if (ft_isequal(str, "unset"))
		return (5);
	if (ft_isequal(str, "env"))
		return (6);
	if (ft_isequal(str, "exit"))
		return (7);
	return (0);
}
