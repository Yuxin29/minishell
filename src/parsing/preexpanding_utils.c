#include "minishell.h"

// return the length of a valid variable name
// not malloc involved
// $ skipped already
int	var_name_len(const char *str)
{
	int	len;

	len = 0;
	if (!str[len])
		return (len);
	if (!ft_check_valid_var_name(str[len]))
		return (len);
	while (ft_check_valid_var_name(str[len]))
		len++;
	return (len);
}

// trackign and switch quotes status
// return 1 if quoted, 0 if not quoted
// quotes[0] -> single quotes
// quotes[1] -> double quotes
int	handle_quotes(char c, int quotes[2], char *res, int *j)
{
	if (c == '\'' && !quotes[1])
	{
		quotes[0] = !quotes[0];
		res[*j] = c;
		(*j)++;
		return (1);
	}
	else if (c == '"' && !quotes[0])
	{
		quotes[1] = !quotes[1];
		res[*j] = c;
		(*j)++;
		return (1);
	}
	return (0);
}

// Bash behavior
// cat << $USER, not expansion, but expanded in the heredoc files
// cat << ""$USER, not expansion anywhere, ""removed
// cat << $USER"", not expansion anywhere, ""removed
// cat << "$USER", not expansion anywhere, "     "removed
// cat << "$USE"R, not expansion anywhere, "    "removed
// cat << "$USER, waiting for second quote, in minishell syntax error
// in minishell
// heredoc is skiped in expanding
// return 1 if heredoc found & processed, return 0 if not a heredoc
int	handle_heredoc_skip(char *raw_line, int ids[2], char *res)
{
	int		in_sq;
	int		in_dq;

	if (raw_line[ids[0]] == '<' && raw_line[ids[0] + 1] == '<')
	{
		res[ids[1]++] = raw_line[ids[0]++];
		res[ids[1]++] = raw_line[ids[0]++];
		while (raw_line[ids[0]] == ' ' || raw_line[ids[0]] == '\t')
			res[ids[1]++] = raw_line[ids[0]++];
		in_sq = 0;
		in_dq = 0;
		while (raw_line[ids[0]] != '\0')
		{
			if (!in_sq && !in_dq && is_empty_or_redirect(raw_line[ids[0]]))
				break ;
			if (raw_line[ids[0]] == '\'' && !in_dq)
				in_sq = !in_sq;
			else if (raw_line[ids[0]] == '"' && !in_sq)
				in_dq = !in_dq;
			res[ids[1]++] = raw_line[ids[0]++];
		}
		return (1);
	}
	return (0);
}

// handke $"..."
// in bash $" is not expanded, copy directly
int	handle_dollar_dquote(char *raw_line, int ids[2], char *res)
{
	if (raw_line[ids[0]] == '$' && raw_line[ids[0] + 1] == '"')
	{
		res[(ids[1])++] = raw_line[(ids[0])++];
		res[(ids[1])++] = raw_line[(ids[0])++];
		while (raw_line[ids[0]] && raw_line[ids[0]] != '"')
			res[(ids[1])++] = raw_line[(ids[0])++];
		if (raw_line[ids[0]] == '"')
			res[(ids[1])++] = raw_line[(ids[0])++];
		return (1);
	}
	return (0);
}

// $? expanded here directly
int	handle_exit_status(char *raw_line, int ids[2], char *res, t_exec_path *cmd)
{
	int		k;
	char	*exit_status_str;

	k = 0;
	if (raw_line[ids[0]] == '$' && raw_line[ids[0] + 1] == '?')
	{
		exit_status_str = ft_itoa(cmd->exit_status);
		if (!exit_status_str)
		{
			perror("malloc");
			return (0);
		}
		while (exit_status_str[k])
		{
			res[(ids[1])++] = exit_status_str[k];
			k++;
		}
		ids[0] += 2;
		free(exit_status_str);
		return (1);
	}
	return (0);
}
