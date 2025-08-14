#include "minishell.h"

static void	append_to_res(char *res, int *res_idx, const char *val)
{
	size_t	k;

	k = 0;
	while (val[k])
		res[(*res_idx)++] = val[k++];
}

// success, expanded
// not an env var to expand here
//if (!key) return (0); //malloc fails, need to perror here
// val = get_env_value(envp, key); //should null check, failure check
int	try_expand_env_var(char *raw_line, int idx[2], char *res, char **envp)
{
	int		len;
	char	*val;
	char	*key;

	if (raw_line[idx[0]] == '$'
		&& ft_check_valid_var_name(raw_line[idx[0] + 1]))
	{
		len = var_name_len(raw_line + idx[0] + 1);
		key = ft_substr(raw_line, idx[0] + 1, len);
		if (!key)
		{
			perror("malloc: ");
			return (0);
		}
		val = get_env_value(envp, key);
		free(key);
		idx[0] += len + 1;
		if (!val)
			return (0);
		append_to_res(res, &idx[1], val);
		free(val);
		return (1);
	}
	return (0);
}

static int	skip_copy(char *raw_line, int idx[2], char *res, int quotes[2])
{
	if (handle_quotes(raw_line[idx[0]], quotes, res, &idx[1]))
	{
		idx[0]++;
		return (1);
	}
	if (quotes[0])
	{
		res[idx[1]++] = raw_line[idx[0]++];
		return (1);
	}
	return (0);
}

void	expand_loop(char *raw_line, char *res, int idx[2], t_exec_path *cmd)
{
	int		quotes[2];
	int		skip;

	skip = 0;
	quotes[0] = 0;
	quotes[1] = 0;
	while (raw_line[idx[0]])
	{
		if (skip_copy(raw_line, idx, res, quotes))
			continue ;
		if (!quotes[0] && !quotes[1]
			&& handle_heredoc_skip(raw_line, idx, &skip, res))
			continue ;
		if (handle_dollar_dquote(raw_line, idx, res))
			continue ;
		if (handle_exit_status(raw_line, idx, res, cmd))
			continue ;
		if (try_expand_env_var(raw_line, idx, res, cmd->envp))
			continue ;
		res[idx[1]++] = raw_line[idx[0]++];
	}
}

// used before lexing, to solve presaved cmd like $A = "echo hello"
// can handle $A$B$C as well
// not doing anything to << heredoc
	// idx[0] = 0; // i
	// idx[1] = 0; // j
char	*pre_expand_line(t_exec_path *cmd, char *raw_line)
{
	char	*res;
	int		idx[2];

	idx[0] = 0;
	idx[1] = 0;
	res = malloc(BUF_SIZE);
	if (!res)
		return (free_malloc_fail_null(NULL));
	expand_loop(raw_line, res, idx, cmd);
	res[idx[1]] = '\0';
	return (res);
}
