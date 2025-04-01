/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:48:33 by gfontagn          #+#    #+#             */
/*   Updated: 2025/04/01 13:49:18 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>

char	*ft_strjoin_slash(char const *s1, char const *s2)
{
	char	*pt;
	size_t	total_len;
	size_t	s1_len;

	if (!s1 || !s2)
		return (0);
	s1_len = ft_strlen((char *)s1);
	total_len = s1_len + ft_strlen((char *)s2) + 1;
	pt = malloc((total_len + 1) * sizeof(char));
	if (!pt)
		return (NULL);
	ft_strlcpy(pt, (char *)s1, total_len + 1);
	pt[s1_len] = '/';
	pt[s1_len + 1] = '\0';
	ft_strlcat(pt, (char *)s2, total_len + 1);
	return (pt);
}

int	is_path_found(char *path)
{
	int	fd;

	if (!path)
		return (0);
	fd = open(path, O_DIRECTORY);
	if (fd != -1)
	{
		close(fd);
		return (0);
	}
	if (access(path, F_OK) == 0)
		return (1);
	return (0);
}

char	*look_for_path(char *name, char **com_paths)
{
	char	*path;
	int	i;

	i = 0;
	if (!name)
		return (NULL);
	if (is_path_found(name))
		return (ft_strdup(name));
	else if (name && name[0] == '/')
	{
		ft_printf("minishell: %s: no such file or directory\n", name);
		return (NULL);
	}
	while (paths[i])
	{
		path = ft_strjoin_slash(paths[i], cmd_name);
		if (!path || is_path_found(path))
			return (path);
		(free(path), i++);
	}
	ft_printf("minishell: %s: command not found\n", cmd_name);
	return (NULL);
}

char	**get_all_paths(t_env_list *env)
{
	char	*path_line;
	char	**paths;

	path_line = ft_getenv("PATH", env);
	if (!path_line)
		return (NULL);
	paths = ft_split(path_line, ':');
	free(path_line);
	if (!paths)
		return (NULL);
	return (paths);
}

char	*find_path(char *name, t_minishell *sh)
{
	char	**common_paths;
	char	*path;

	if (ft_strlen(name) == 0)
		return (NULL);
	common_paths = get_all_paths(sh->env_list);
	path = look_for_path(args[0], common_paths);
	if (common_paths)
		free_args(common_paths);
	return (path);
}
