#include "parsing.h"
#include "minishell.h"

//redirections.c
//after parsing, we got already cmd list.
//we check here if there are redirections symbles here in each cmd.
void	check_and_apply_redirections(t_cmd *cmd)
{
	int	fd;
    
	if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
		{
			perror(cmd->infile);
			exit(1);//is exit status 1 when infile faild???
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (cmd->outfile)
	{
		if(cmd->append_out == 1)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (cmd->append_out == 0)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror(cmd->outfile);
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

/* 
#include <stdio.h>
mkstemp - create a unique temporary file
int mkstemp(char *template); ///XXXXXX are ramdom characters here
The  mkstemp() function generates a unique temporary filename from template, 
creates and opens the file, and returns an open file  descriptor for the file.
snprintf - snprintf
int snprintf(char *str, size_t size, const char *format, ...);
The  functions snprintf() write at most size bytes (including the terminating null byte ('\0')) to str.
*/

static void	get_tmp_file(t_cmd *cmd_list, int heredoc_count, char *tmp_path, int fd)
{
	tmp_path = malloc(32);
	if (!tmp_path)
	{
		free_cmd_list(cmd_list);
		perror("malloc");
		exit(1);
	}
	snprintf(tmp_path, 32, "/tmp/.heredoc_%d_XXXXXX", heredoc_count++);
	fd = mkstemp(tmp_path);
	if (fd < 0)
	{
		free(tmp_path);
		free_cmd_list(cmd_list);
		perror(cmd_list->heredoc_delim);
		exit(1);
	}
}

void        check_and_apply_heredocs(t_cmd *cmd_list)
{
	char	*tmp_path;  //what happens if the are pipes or multiple commands, is the tmp file reused, does the mkstemp happen at the same time and same path, or with an first and second order
	int		fd;
	char	*line;
	int		heredoc_count;

	tmp_path = NULL;
	heredoc_count = 0;
	fd = 0;
	while (cmd_list)
	{
		if (cmd_list->heredoc_delim)
		{
			get_tmp_file(cmd_list, heredoc_count, tmp_path, fd);
			while (1)
			{
				line = readline("minishell: heredoc> ");
				if (!line || ft_strcmp(line, cmd_list->heredoc_delim) == 0)//Stop reading when user inputs a line exactly matching the heredoc delimiter.
				{
					free(line);
					break ;
				}
				ft_putendl_fd(line, fd);//Write all lines except the delimiter into the temporary file.
				free(line);
			}
			close(fd);
			cmd_list->infile = tmp_path;// Update infile to temp path
		}
		cmd_list = cmd_list->next;
	}
}

