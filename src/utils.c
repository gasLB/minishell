/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 19:35:03 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/20 16:44:24 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>

int	is_equal(char *s1, char *s2)
{
	if (ft_strlen(s1) != ft_strlen(s2))
		return (0);
	if (ft_strncmp(s1, s2, ft_strlen(s1)) != 0)
		return (0);
	return (1);
}

char	*ft_realloc_str(char *str, int new_size) // Useless for now
{
	char	*new_str;
	int	i;

	if ((int)ft_strlen(str) > new_size)
		return (NULL);
	if (!str)
		return (NULL);
	new_str = ft_calloc(new_size, sizeof(char));
	if (!new_str)
		return (NULL);
	i = -1;
	while (str[i++])
		new_str[i] = str[i];
	free(str);
	return (new_str);
}

char	*append_str(char *dest, char *src)
{
	char	*new;

	if (!dest)
		return (NULL);
	if (!src)
		return (NULL);
	new = ft_strjoin(dest, src);
	if (!new)
		return (NULL);
	free(dest);
	free(src);
	return (new);
}
