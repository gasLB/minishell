/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:50:00 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/11 18:14:36 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

int	is_n_option(char *s)
{
	int	i;
	int	size;

	size = ft_strlen(s);
	if (ft_strncmp(s, "-n", 2) != 0)
		return (0);
	i = 2;
	while (i < size)
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	change_directories(char *path)
{
	if (chdir(path) != 0)
	{
		ft_printf("minishell: cd: %s: No such file or directory\n", path);
		exit(1);
	}
	exit(0);
}
