#include "minishell.h"

// success, expanded
// not an env var to expand here
//if (!key) return (0); //malloc fails, need to perror here
// val = get_env_value(envp, key); //should null check, failure check
int	try_expand_env_var(char *raw_line, int *i, char *res, int *j, char **envp)
{
	int		len;
	char	*val;
	size_t	k;
	char	*key;

	if (raw_line[*i] == '$' && raw_line[*i + 1] && ft_check_valid_var_name(raw_line[*i + 1]))
	{
		len = var_name_len(raw_line + *i + 1);
		key = ft_substr(raw_line, *i + 1, len);
		if (!key)
			return (0);
		val = get_env_value(envp, key);
		free (key);
		*i += len + 1;
		if (!val)
			return (0);
		k = 0;
		while (val[k])
			res[(*j)++] = val[k++];
		free (val);
		return (1);
	}
	return (0);
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
int	handle_heredoc_skip(char *raw_line, int *i, int *skip_expand, char *res, int *j)
{
	if (!(*skip_expand) && raw_line[*i] == '<' && raw_line[*i + 1] == '<')
	{
		*skip_expand = 1;
		res[*j] = raw_line[*i];
		(*j)++;
		(*i)++;
		res[*j] = raw_line[*i];
		(*j)++;
		(*i)++;
		return (1);
	}
	if (*skip_expand)
	{
		res[*j] = raw_line[*i];
		(*j)++;
		if (ft_isspace(raw_line[*i]))
			*skip_expand = 0;
		(*i)++;
		return (1);
	}
	return (0);
}

int	handle_dollar_dquote(char *raw_line, int *i, char *res, int *j)
{
	if (raw_line[*i] == '$' && raw_line[*i + 1] == '"')
	{
		res[(*j)++] = raw_line[(*i)++];
		res[(*j)++] = raw_line[(*i)++];
		while (raw_line[*i] && raw_line[*i] != '"')
			res[(*j)++] = raw_line[(*i)++];
		if (raw_line[*i] == '"')
			res[(*j)++] = raw_line[(*i)++];
		return (1);
	}
	return (0);
}

int	handle_exit_status(char *raw_line, int *i, char *res, int *j, char *exit_status_str)
{
	int	k;

	k = 0;
	if (raw_line[*i] == '$' && raw_line[*i + 1] == '?')
	{
		while (exit_status_str[k])
		{
			res[(*j)++] = exit_status_str[k];
			k++;
		}
		*i += 2;
		return (1);
	}
	return (0);
}
