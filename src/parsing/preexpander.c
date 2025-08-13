#include "minishell.h"

//return the length of a valid variable name
//ft_isalnum, not sure, it should be only uppper case
int	var_name_len(const char *str)
{
	int	len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (len);
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

	i = 0;
	j = 0;
	quotes[0] = 0;
	quotes[1] = 0;
	skip_expand = 0;
	exit_status_str = ft_itoa(cmd->exit_status);
	if (!exit_status_str)
		return (free_malloc_fail_null(NULL));
	res = malloc(BUF_SIZE);
	if (!res)
		return (free_malloc_fail_null(exit_status_str));
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
		if (handle_dollar_dquote(raw_line, &i, res, &j)
			|| handle_exit_status(raw_line, &i, res, &j, exit_status_str)
			|| try_expand_env_var(raw_line, &i, res, &j, envp))
			continue ;
		res[j++] = raw_line[i++];
	}
	res[j] = '\0';
	free(exit_status_str);
	return (res);
}
