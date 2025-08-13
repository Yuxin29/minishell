#include "minishell.h"

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
	int		saved_stdin;

	tmp_file = get_tmp_filepath();
	if (!tmp_file)
		return (perror("malloc: "), NULL);
	fd = open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		return (free(tmp_file), perror("heredoc open failed"), NULL);
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin < 0)
		return (close(fd), free(tmp_file), perror("dup"), NULL);
	signal_heredoc();
	while (1)
	{
		line = readline("minishell heredoc> ");
		if (g_signal == 1 || !line)
			break ;
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	signal_init();
	if (!dup2(saved_stdin, STDIN_FILENO))
	{
		perror("dup2");
		close(saved_stdin);
		close(fd);
		unlink(tmp_file);
		free(tmp_file);
		return (NULL);
	}
	close(saved_stdin);
	close(fd);
	if(g_signal)
	{
		unlink(tmp_file);
		free(tmp_file);
		return (NULL);
	}
	return (tmp_file);
}
