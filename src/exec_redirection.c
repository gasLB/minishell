/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:53:55 by gfontagn          #+#    #+#             */
/*   Updated: 2025/04/16 15:34:01 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int	open_in(char **args, char *file_name, int in_status, t_minishell *sh)
{
	int	infile;

	infile = -1;
	if (in_status == HD)
		return (here_doc(file_name, args, sh));
	if (access(file_name, F_OK) != 0)
		printf_fd(2, "%s: %s: No such file or directory\n", args[0], file_name);
	else
	{
		infile = open(file_name, O_RDONLY, 0644);
		if (infile == -1)
			printf_fd(2, "%s: %s: Permission denied\n", args[0], file_name);
	}
	return (infile);
}

int	open_out(char **args, char *file_name, int out_status)
{
	int	outfile;

	outfile = -1;
	if (access(file_name, F_OK) == 0 && access(file_name, W_OK) != 0)
	{
		printf_fd(2, "%s: %s: Permission denied\n", args[0], file_name);
		return (-1);
	}
	if (out_status == TRUNC)
		outfile = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if (out_status == APPEND)
		outfile = open(file_name, O_WRONLY | O_APPEND | O_CREAT, 0644);
	return (outfile);
}

int	set_redirections(char **args, t_redir_node *redir, t_minishell *sh)
{
	int	file;
	int	type;

	if (!redir || redir->type == -1)
		return (0);
	type = redir->type;
	if (type == IN || type == HD)
	{
		file = open_in(args, redir->str, type, sh);
		if (file == -1 || dup2(file, STDIN_FILENO) == -1)
			return (1);
	}
	else if (type == TRUNC || type == APPEND)
	{
		file = open_out(args, redir->str, type);
		if (file == -1 || dup2(file, STDOUT_FILENO) == -1)
			return (1);
	}
	else
		return (1);
	close(file);
	return (0);

}

/*

CHAIN OF REDIRECT

What are the rules?

-> We traverse the redirection chain
	-> for each node, check validity. If valid:
		-> for OUT: if doesn't exist we create the node.
		-> we open the corresponding file
		-> we duplicate the fd on corresponding std
		-> we close the file fd
	-> if we find an invalid nodem stop the whole command

EXAMPLES

touch file
chmod 000 file
ls > file2 < file > file4
-> file2 is created
-> file : permission denied
-> stops

Actually we can have mulitple redirections in line

OUT:

! if there is an error in IN, OUT is not taken into account
wait that might have to do with the order
-> yes it is excatly that
	For what I understand, each redirect node is evaluated in line. If one fails, it stops the execution of all followig redirects
only the last is taken to account but the previous ones create the files
Actally no, the middle file are emptied for TRUNC

IN:
Only the last redirection is taken into account

same shit

EXAMPLES

cat > b > c < test

for OUT, 2nd file is not created if error before in the pipeline

for IN
typedef struct s_red_node
{
	int	type;	// HD or IN or TRUNC or APPEND
	char	*str;	// file name or LIM (heredoc)
	struct s_red_node	*next;
}	t_red_node;

typedef struct s_redirect
{
	struct s_red_node	*in;
	struct s_red_node	*out;
}	t_redirect;
*/
