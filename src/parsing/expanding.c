#include "minishell.h"

// return the value of a key from the envp
// special case: split[1] == NULL (e.g. FOO= in envp).
char	*get_env_value(char **envp, const char *key)
{
	int		j;
	char	**split;
	char	*new_value;

	j = 0;
	while (envp[++j])
	{
		split = ft_split(envp[j], '=');
		if (!split || !*split)
		{
			if (!split)
				ft_free_arr(split);
			return (free_malloc_fail_null(NULL));
		}
		if (ft_strcmp(split[0], (char *)key) == 0)
		{
			new_value = ft_strdup(split[1]);
			ft_free_arr(split);
			if (!new_value)
				return (free_malloc_fail_null(NULL));
			return (new_value);
		}
		ft_free_arr(split);
	}
	return (NULL);
}

char	*get_env_value_from_substr(char *input, int start, int len, char **envp)
{
	char	*var_name;
	char	*value;

	var_name = ft_substr(input, start, len);
	if (!var_name)
		return (NULL);
	value = get_env_value(envp, var_name);
	free(var_name);
	return (value);
}

// examlpe of variable expandable in a redirection, and this variable is $?
// ls > output_$?.txt
// cat output_0.txt
// value = get_env_value_from_substr(input, start, var_len, envp);
//  need a spacial null check for this mem failure or ""
	// if (!value)
	// 	value = ft_strdup(""); //null check or macro
char	*replace_variable(t_exec_path *cmd, char *input, int pos, char **envp)
{
	int		start;
	int		var_len;
	char	*value;
	char	*prefix;
	char	*suffix;

	(void)cmd;
	start = pos + 1;
	var_len = 0;
	while (ft_check_valid_var_name(input[start + var_len]))
		var_len++;
	value = get_env_value_from_substr(input, start, var_len, envp);
	if (!value)
		value = ft_strdup("");
	prefix = ft_substr(input, 0, pos);
	suffix = ft_strdup(input + start + var_len);
	if (!prefix)
		return (free(value), free_malloc_fail_null(suffix));
	if (!suffix)
		return (free(value), free_malloc_fail_null(prefix));
	return (join_three_and_free(prefix, value, suffix));
}

// the 2 free (input);
// with it, it is still reachable, without it, it is definitly loss
// echo $? already expanded in preexpander
// return (replace_variable(cmd, input, i, envp));null is included in this one
		// if (input[i] == '$' && ft_check_valid_var_name(input[i + 1]))
		// 	return (replace_variable(cmd, input, i, envp)); // FIX
char	*expand_variables_in_str(t_exec_path *cmd, char *input, char **envp)
{
	int		i;

	i = 0;
	if (!input)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '$' && ft_check_valid_var_name(input[i + 1]))
			return (replace_variable(cmd, input, i, envp));
		else
			i++;
	}
	return (input);
}

//call this after getting the cmd list
// I only need to expand << str here, 
void	expand_all_cmds(t_exec_path *cmd, t_cmd *cmd_list, char **envp)
{
	t_redir	*redir;
	char	*value;

	while (cmd_list)
	{
		redir = cmd_list->redirections;
		while (redir)
		{
			if (redir->type == 5 && redir->quoted == 0 && redir->heredoc_delim)
			{
				if (ft_strchr(redir->heredoc_delim, '$'))
				{
					value = expand_variables_in_str(cmd, redir->heredoc_delim, envp);
					if (!value)
						return ;
					if (value != redir->heredoc_delim)
					{
						free(redir->heredoc_delim);
						redir->heredoc_delim = value;
					}
				}
			}
			redir = redir->next;
		}
		cmd_list = cmd_list->next;
	}
}
