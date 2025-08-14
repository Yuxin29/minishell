#include "minishell.h"

//return the length of a valid variable name
//ft_isalnum, not sure, it should be only uppper case
int	var_name_len(const char *str)
{
	int	len;

	len = 0;
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (len);
}

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

//heredoc
int	handle_heredoc_skip(char *raw_line, int ids[2], int *skip_expand, char *res)
{
	if (!(*skip_expand)
		&& raw_line[ids[0]] == '<' && raw_line[ids[0] + 1] == '<')
	{
		*skip_expand = 1;
		res[ids[1]] = raw_line[ids[0]];
		(ids[1])++;
		(ids[0])++;
		res[ids[1]] = raw_line[ids[0]];
		(ids[1])++;
		(ids[0])++;
		return (1);
	}
	if (*skip_expand)
	{
		res[ids[1]] = raw_line[ids[0]];
		(ids[1])++;
		if (ft_isspace(raw_line[ids[0]]))
			*skip_expand = 0;
		(ids[0])++;
		return (1);
	}
	return (0);
}

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
			perror("malloc: ");
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
