#include "minishell.h"

redirections.c
after parsing, we got already cmd list.
we check here if there are redirections symbles here in each cmd node.
static int	apply_infile(t_cmd *cmd)
{
	int	fd;

	fd = open(cmd->infile, O_RDONLY);
	if (fd < 0)
		return (perror(cmd->infile), -1);
	if (ft_strncmp(cmd->infile, "/tmp/.heredoc_", 14) == 0)
		unlink(cmd->infile);
	if (dup2(fd, STDIN_FILENO) < 0)
		return (perror("dup2 infile failed"), close(fd), -1);
	close(fd);
	return (0);
}

static int	apply_outfile(t_cmd *cmd)
{
	int	fd;

	if (cmd->append_out == 1)
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(cmd->outfile), -1);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (perror("dup2 outfile failed"), close(fd), -1);
	close(fd);
	return (0);
}

int	check_and_apply_redirections(t_cmd *cmd)
{
	if (!cmd)
		return (-1);
	if (cmd->infile)
		if (apply_infile(cmd) < 0)
			return (-1);
	if (cmd->outfile)
		if (apply_outfile(cmd) < 0)
			return (-1);
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
	return (tmp_file);
	// return tmpfile, in token, check if t_type==5, then cmd_list->infile== tmp_file, then do redirections
}
