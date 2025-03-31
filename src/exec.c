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

int	cmd_node(t_ast_node *node, t_minishell *sh)
{
	t_token	**token_list;
	char	**args;

	token_list = populate_tokens(node->ac, node->args);	// we could do without ac ! (NULL-trm)
	token_list = expand_tokens(token_list, sh, sh->env_list);
	args = expanded_list(node->ac, token_list);
	// free token_list here
	if (is_builtin(args[0]))
		exec_builtin(args, node, sh);
	else
	{
		args[0] = find_path(args[0], sh);
		if (!args[0])
			(free_args(args), return (1));
		return (exec_cmd(args, node, sh));
	}
}



	// 1. Is it a builtin?
	// 2. Is it available in CWD?
	// 3. Is it in the path?
/*

What are the steps for executing one command?
1. tokenize and expand the args into tk_list
2. creat new args with expanded_list()
3. free tk_list
4. execute with args


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
