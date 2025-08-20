#include "minishell.h"

// there might be overflow when char length beyond BUFF_SIZE
static void	append_to_res(char *res, int *res_idx, const char *val)
{
	size_t	k;

	k = 0;
	while (val[k])
		res[(*res_idx)++] = val[k++];
}

// 1 for success, expanded
// 0 for not expanded, skiped
// if (!val), not found or EMPTY found, -> ""
// success, expanded
// not an env var to expand here
// if (!key) return (0); //malloc fails, need to perror here ??
// val: null check, checked
int	try_expand_env_var(char *raw_line, int idx[2], char *res, t_exec_path *cmd)
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
			errmsg_return_nbr("malloc", 1, 0);
		val = get_env_value(cmd->envp, key);
		free(key);
		idx[0] += len + 1;
		if (!val)
			val = ft_strdup("");
		if (!val)
			errmsg_return_nbr("malloc", 1, 0);
		append_to_res(res, &idx[1], val);
		if (val)
			free(val);
		return (1);
	}
	return (0);
}

// yuxin added
static int	handle_redir_skip(char *raw_line, int ids[2], char *res)
{
	if (!(raw_line[ids[0]] == '<' || raw_line[ids[0]] == '>'))
		return (0);
	res[ids[1]++] = raw_line[ids[0]++];
	if ((res[ids[1] - 1] == '<' && raw_line[ids[0]] == '<')
		|| (res[ids[1] - 1] == '>' && raw_line[ids[0]] == '>'))
		res[ids[1]++] = raw_line[ids[0]++];
	while (raw_line[ids[0]] == ' ' || raw_line[ids[0]] == '\t')
		res[ids[1]++] = raw_line[ids[0]++];
	while (raw_line[ids[0]] && !is_empty_or_redirect(raw_line[ids[0]]))
		res[ids[1]++] = raw_line[ids[0]++];
	return (1);
}

void	expand_loop(char *raw_line, char *res, int idx[2], t_exec_path *cmd)
{
	int		quotes[2];

	memset(quotes, 0, sizeof(quotes));
	while (raw_line[idx[0]])
	{
		if (handle_quotes(raw_line[idx[0]], quotes, res, &idx[1]))
		{
			idx[0]++;
			continue ;
		}
		if (quotes[0])
		{
			res[idx[1]++] = raw_line[idx[0]++];
			continue ;
		}
		if (!quotes[0] && !quotes[1] && handle_heredoc_skip(raw_line, idx, res))
			continue ;
		if (!quotes[0] && !quotes[1] && handle_redir_skip(raw_line, idx, res))
			continue ;
		if (handle_exit_status(raw_line, idx, res, cmd))
			continue ;
		if (try_expand_env_var(raw_line, idx, res, cmd))
			continue ;
		res[idx[1]++] = raw_line[idx[0]++];
	}
}

// used before lexing, to solve presaved cmd like $A = "echo hello"
// can handle $A$B$C as well
// not doing anything to << heredoc
// idx[0] = 0; // i, current position in raw_line
// idx[1] = 0; // j, current position in res (output buffer)
char	*pre_expand_line(t_exec_path *cmd, char *raw_line)
{
	char	*res;
	int		idx[2];

	idx[0] = 0;
	idx[1] = 0;
	res = malloc(LINE_SIZE);
	if (!res)
		return (free_malloc_fail_null(NULL));
	expand_loop(raw_line, res, idx, cmd);
	res[idx[1]] = '\0';
	return (res);
}
