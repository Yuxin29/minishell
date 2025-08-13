#include "minishell.h"

//return the length of a valid variable name
//ft_isalnum, not sure, it should be only uppper case
int	var_name_len(const char *str)
{
	int	len;

	len = 0;
	// if (str[len] == '$')
	// 	len++;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (len);
}

// success, expanded
// not an env var to expand here
int	try_expand_env_var(char *raw_line, int *i, char *res, int *j, char **envp)
{
	int		len;
	char	*val;
	size_t	k;
	char	*key;

	if (raw_line[*i] == '$' && raw_line[*i + 1] && (ft_isalnum(raw_line[*i + 1]) || raw_line[*i + 1] == '_'))
	{
		len = var_name_len(raw_line + *i + 1);
		key = ft_substr(raw_line, *i + 1, len);
		if (!key)
			return (0); //malloc fails, need to perror here
		val = get_env_value(envp, key); //should null check, failure check
		free (key);
		*i += len + 1;
		if (!val) //not good, need to make sure it is not from malloc failure
            val = strdup("");
		if (val)
		{
			k = 0;
			while (val[k])
				res[(*j)++] = val[k++];
			free (val);
		}
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

// used before lexing, to solve presaved cmd like $A = "echo hello"
// can handle $A$B$C as well
// not doing anything to << heredoc
char	*pre_expand_line(t_exec_path *cmd, char *raw_line, char **envp)
{
	char	*res;
	int		i;
	int		j;
	int		quotes[2];
	int		skip_expand;
	char	*exit_status_str;
	int		k;

	exit_status_str = ft_itoa(cmd->exit_status);
	(void)cmd;

	i = 0;
	j = 0;
	quotes[0] = 0;
	quotes[1] = 0;
	skip_expand = 0;
	res = malloc(BUF_SIZE);
	if (!res)
		return (NULL);
	while (raw_line[i])
	{
		if (handle_quotes(raw_line[i], quotes, res, &j))
		{
			i++;
			continue ;
		}
		if (!quotes[0] && !quotes[1] && handle_heredoc_skip(raw_line, &i, &skip_expand, res, &j))
			continue ;
		if (quotes[0])
		{
			res[j++] = raw_line[i++];
			continue ;
		}
		if (raw_line[i] == '$' && raw_line[i + 1] == '"')
		{
			res[j++] = raw_line[i++];
			res[j++] = raw_line[i++];
			while (raw_line[i] && raw_line[i] != '"')
				res[j++] = raw_line[i++];
			if (raw_line[i] == '"')
				res[j++] = raw_line[i++];
			continue ;
		}
		if (raw_line[i] == '$' && raw_line[i + 1] == '?')
		{
			exit_status_str = ft_itoa(cmd->exit_status);
			k = 0;
			while (exit_status_str[k])
				res[j++] = exit_status_str[k++];
			i += 2;
			continue ;
		}
		if ((try_expand_env_var(raw_line, &i, res, &j, envp)))
			continue ;
		else
			res[j++] = raw_line[i++];
	}
	res[j] = '\0';
	free(exit_status_str);
	return (res);
}
