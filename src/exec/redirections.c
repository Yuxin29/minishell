#include "minishell.h"

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
		return (-1);
	}
	close(fd);
	return (0);
}

static int	apply_outfile(t_redir *redir, int is_last)
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
		{
			perror("dup2 outfile failed");
			close(fd);
			return (-1);
		}
	}
	close(fd);
	return (0);
}

static int	apply_out_or_in(t_redir *cur, t_redir *last)
{
	int	is_last;

	if (cur->type == 2 || cur->type == 5)
	{
		if (apply_infile(cur) < 0)
			return (0);
	}
	else if (cur->type == 3 || cur->type == 4)
	{
		if (cur == last)
			is_last = 1;
		else
			is_last = 0;
		if (apply_outfile(cur, is_last) < 0)
			return (0);
	}
	return (1);
}

int	check_and_apply_redirections(t_cmd *cmd)
{
	t_redir	*cur;
	t_redir	*last;

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
		if (cur->is_ambiguous)
			return (errmsg_return_nbr(SYNTAX_ERR_AMBIGUOUS, 0, -1));
		if (!apply_out_or_in(cur, last))
			return (-1);
		cur = cur->next;
	}
	return (0);
}
