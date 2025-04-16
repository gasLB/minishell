/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:31:39 by gfontagn          #+#    #+#             */
/*   Updated: 2025/04/16 15:03:01 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

static int	putncount_fd(int fd, char c)
{
	ft_putchar_fd(c, fd);
	return (1);
}

static char	*increment_s(char *s)
{
	s++;
	while (*s && !ft_strchr("cspdiuxX%", *s))
		s++;
	return (s);
}

int	printf_fd(int fd, const char *s, ...)
{
	va_list	ap;
	int		count;
	int		ret;

	va_start(ap, s);
	count = 0;
	while (*s)
	{
		if (*s == '%' && *(s + 1))
		{
			ret = f_found((char *)(s), ap);
			if (ret != -1)
			{
				count += ret;
				s = increment_s((char *)s);
			}
		}
		else
			count += putncount_fd(fd, *s);
		if (*s)
			s++;
	}
	va_end(ap);
	return (count);
}
