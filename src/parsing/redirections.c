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
tmpfile - create a temporary file
FILE *tmpfile(void);

#include <stdio.h>
mkstemp - create a unique temporary file
int mkstemp(char *template);
The  mkstemp() function generates a unique temporary filename from template, 
creates and opens the file, and returns an open file  descriptor for the file.
*/

void        check_and_apply_heredocs(t_cmd *cmd_list)
{
	char	tmp_path[] = "/tmp/.heredoc_XXXXXX";  //what happens if the are pipes or multiple commands, is the tmp file reused, does the mkstemp happen at the same time and same path, or with an first and second order
	int		fd;
	char	*line;

	while (cmd_list)
	{
		if (cmd_list->heredoc_delim)
		{
			fd = mkstemp(tmp_path);
			if (fd < 0)
			{
				free_cmd_list(cmd_list);
				perror(cmd_list->heredoc_delim);
				exit(1);
			}
			while (1)
			{
				line = readline("minishell: heredoc> ");
				if (!line || ft_strncmp(line, cmd_list->heredoc_delim, ft_strlen(cmd_list->heredoc_delim)) == 0)//Stop reading when user inputs a line exactly matching the heredoc delimiter.
				{
					free(line);
					break ;
				}
				write(fd, line, ft_strlen(line));//Write all lines except the delimiter into the temporary file.
				write(fd, "\n", 1);//Update the command’s infile field to the temporary file path, replacing any previous infile.
				free(line);
			}
			close(fd);
			cmd_list->infile = ft_strdup(tmp_path);// Update infile to temp path
		}
		cmd_list = cmd_list->next;
	}
}
