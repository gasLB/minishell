#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <termios.h>

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	read_print_input(void)
{
	char *rl;

	rl = readline("Prompt > ");
	if (rl)
		add_history(rl);
	while (*rl)
	{
		rl = readline("Prompt > ");
		add_history(rl);
	}
	printf("%s\n", rl);
	free(rl);	
}

void	move_up(void)
{
	char	*cwd;

	if ((cwd = getcwd(NULL, 0)) != NULL)
		printf("current dir : %s\n", cwd);
	if (chdir("..") != 0)
		perror("chdir failed");
	else
		printf("chdir succeed");
	if ((cwd = getcwd(NULL, 0)) != NULL)
		printf("new dir : %s\n", cwd);
}

void	get_stats(void)
{
	struct stat	file_info;

	if (stat("preliminary_notes.md", &file_info) == -1)
		perror("stat failed\n");
	printf("file size : %ld\n", file_info.st_size);
	printf("permissions: %o\n", file_info.st_mode & 0777);
	// warning: need to differenciate directories and files
}

void	simple_ls(void)
{
	DIR	*dp;
	struct dirent	*ep;

	dp = opendir("./");
	if (dp != NULL)
	{
		while (1)
		{
			ep = readdir(dp);
			if (ep == NULL)
				break;
			printf("%s\n", ep->d_name);
		}
		closedir(dp);
	}
	else
		perror ("Couldn't open the directory");
}

void	get_terminal_size(void)
{
    struct winsize ws;
    
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        perror("ioctl");
        return 1;
    }
    printf("Terminal is %d rows by %d columns\n", 
           ws.ws_row, ws.ws_col);
    return 0;
}

int	main(void)
{
	get_terminal_size();
	return (0);
}
