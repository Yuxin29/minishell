#include "minishell.h"

/*every thread has a unique pid(precess ID), so we can use pid to differ heredoc, for every heredoc we should have a temperary file to save lines, so in this case, cat << out << 1 << 2, we should have 3 different tmp files, so we should use pid to name different tmp filename
than this tmp file should become the infile in the process*/
static char	*get_tmp_filepath(void)
{
	static int	i; //generate unique tmp filename, should be static
	char		*tmp_filepath;
	char		*fake_pid;

	fake_pid = ft_itoa(i++);
	if (!fake_pid)
		return (NULL);
	tmp_filepath = ft_strjoin("/tmp/.heredoc_", fake_pid);
	free(fake_pid);
	if (!tmp_filepath)
		return (NULL);
	return (tmp_filepath);
}

static int	get_tmpfile_fd(char **tmp_file)
{
	int	fd;

	*tmp_file = get_tmp_filepath();
	if (!*tmp_file)
	{
		perror("malloc: ");
		return (-1);
	}
	fd = open(*tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		free(*tmp_file);
		perror("heredoc open failed");
		return (-1);
	}
	return (fd);
}

static int	handle_heredoc_input(int fd, char *delim)
{
	char	*line;

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
	return (g_signal);
}

static int	restore_stdin(int saved_stdin)
{
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(saved_stdin);
		return (0);
	}
	close(saved_stdin);
	return (1);
}

char	*creat_heredoc_file(char *delim)
{
	int		fd;
	char	*tmp_file;
	int		saved_stdin;
	int		interrupted;

	fd = get_tmpfile_fd(&tmp_file);
	if (fd == -1)
		return (NULL);
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin < 0)
		return (cleanup_heredoc(fd, -1, tmp_file, "dup"));
	signal_heredoc();
	interrupted = handle_heredoc_input(fd , delim);
	signal_init();
	if (!restore_stdin(saved_stdin))
		return (cleanup_heredoc(fd, saved_stdin, tmp_file, "dup2"));
	close(fd);
	if (interrupted)
		return (cleanup_heredoc(-1, -1, tmp_file, NULL));
	return (tmp_file);
}
