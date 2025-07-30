#include "minishell.h"

//redirections.c
//after parsing, we got already cmd list.
//we check here if there are redirections symbles here in each cmd node.
int	check_and_apply_redirections(t_cmd *cmd)
{
	int	fd;

	if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
			return (perror(cmd->infile), -1);
		unlink(cmd->infile); //should unlink tmp file(here cod) after open, unlink only affect tmp file
		if (dup2(fd, STDIN_FILENO) < 0)
			return (perror("dup2 infile failed"), close(fd), -1);
		close(fd);
	}
	if (cmd->outfile)
	{
		if (cmd->append_out == 1)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (cmd->append_out == 0)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (perror(cmd->infile), -1);
		if (dup2(fd, STDOUT_FILENO) < 0)
			return (perror("dup2 outfile failed"), close(fd), -1);
		close(fd);
	}
	return (0);
}


/*every thread has a unique pid(precess ID), so we can use pid to differ heredoc, for every heredoc we should have a temperary file to save lines, so in this case, cat << out << 1 << 2, we should have 3 different tmp files, so we should use pid to name different tmp filename
than this tmp file should become the infile in the process*/
static char	*get_tmp_filepath(void)
{
	static int	i; //generate unique tmp filename
	char	*tmp_filepath;
	char	*fakepid;

	fakepid = ft_itoa(i++);
	tmp_filepath = ft_strjoin("/tmp/.heredoc_", fakepid);
	free(fakepid);
	if (!tmp_filepath)
		return (NULL);
	return (tmp_filepath);
}

char	*creat_heredoc_file(char *delim)
{
	int		fd;
	char	*line;
	char	*tmp_file;

	tmp_file = get_tmp_filepath();
	fd = open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd < 0)
		return (free(tmp_file), perror("heredoc open failed"), NULL);
	while (1)
	{
		line = readline("minishell herecod> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	return (tmp_file); // return tmpfile, in token, check if t_type==5, then cmd_list->infile== tmp_file, then do redirections
}


// void	check_and_apply_heredocs(t_cmd *cmd_list)
// {
// 	char	*tmp_path;//what happens if the are pipes or multiple commands, is the tmp file reused, does the mkstemp happen at the same time and same path, or with an first and second order
// 	int		fd;
// 	char	*line;
// 	int		heredoc_count;

// 	tmp_path = NULL;
// 	heredoc_count = 0;
// 	fd = 0;
// 	while (cmd_list)
// 	{
// 		if (cmd_list->heredoc_delim)
// 		{
// 			get_tmp_file(cmd_list, heredoc_count, tmp_path, fd);
// 			while (1)
// 			{
// 				line = readline("minishell: heredoc> ");
// 				if (!line || ft_strcmp(line, cmd_list->heredoc_delim) == 0)//Stop reading when user inputs a line exactly matching the heredoc delimiter.
// 				{
// 					free (line);
// 					break ;
// 				}
// 				ft_putendl_fd(line, fd);//Write all lines except the delimiter into the temporary file.
// 				free(line);
// 			}
// 			close(fd);
// 			cmd_list->infile = tmp_path;// Update infile to temp path
// 		}
// 		cmd_list = cmd_list->next;
// 	}
// }

// static void	get_tmp_file(t_cmd *cmd_list, int heredoc_count, char *tmp_path, int fd)
// {
// 	tmp_path = malloc(32);
// 	if (!tmp_path)
// 	{
// 		free_cmd_list(cmd_list);
// 		perror("malloc");
// 		exit(1);
// 	}
// 	snprintf(tmp_path, 32, "/tmp/.heredoc_%d_XXXXXX", heredoc_count++);
// 	fd = mkstemp(tmp_path);
// 	if (fd < 0)
// 	{
// 		free(tmp_path);
// 		free_cmd_list(cmd_list);
// 		perror(cmd_list->heredoc_delim);
// 		exit(1);
// 	}
// }
