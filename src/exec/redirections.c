#include "minishell.h"

#include <readline/readline.h>
#include <readline/history.h>

// redirections.c
// after parsing, we got already cmd list.
// we check here if there are redirections symbles here in each cmd node.
static int	apply_infile(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd < 0)
		return (perror(redir->file), -1);
	if (ft_strncmp(redir->file, "/tmp/.heredoc_", 14) == 0)
		unlink(redir->file);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2 infile failed");
		close(fd);
		if (ft_strncmp(redir->file, "/tmp/.heredoc_", 14) == 0)
			unlink(redir->file);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	apply_outfile(t_redir *redir , int is_last)
{
	int	fd;

	if (redir->type == 4)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(redir->file), -1);
	if (is_last)
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
			return (perror("dup2 outfile failed"), close(fd), -1);
	}
	close(fd);
	return (0);
}

int	check_and_apply_redirections(t_cmd *cmd)
{
	t_redir	*cur;
	t_redir	*last;
	int		is_last;

	if (!cmd)
		return (-1);
	cur = cmd->redirections;
	last = NULL;
	while (cur)
	{
		if (cur->type == 3 || cur->type == 4)
			last = cur;
		cur = cur->next;
	}
	cur = cmd->redirections;
	while (cur)
	{
		if (cur->type == 2 || cur->type == 5)
		{
			if (apply_infile(cur) < 0)
				return (-1);
		}
		else if (cur->type == 3 || cur->type == 4)
		{
			if (cur == last)
				is_last = 1;
			else
				is_last = 0;
			if (apply_outfile(cur, is_last) < 0)
				return (-1);
		}
		cur = cur->next;
	}
	return (0);
}

/*every thread has a unique pid(precess ID), so we can use pid to differ heredoc, for every heredoc we should have a temperary file to save lines, so in this case, cat << out << 1 << 2, we should have 3 different tmp files, so we should use pid to name different tmp filename
than this tmp file should become the infile in the process*/
static char	*get_tmp_filepath(void)
{
	static int	i; //generate unique tmp filename
	char	*tmp_filepath;
	char	*fake_pid;

	fake_pid = ft_itoa(i++);
	tmp_filepath = ft_strjoin("/tmp/.heredoc_", fake_pid);
	free(fake_pid);
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
	if (!tmp_file)
		return (perror("malloc: "), NULL);
	fd = open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd < 0)
		return (free(tmp_file), perror("heredoc open failed"), NULL);
	while (1)
	{
		line = readline("minishell heredoc> ");
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
}
