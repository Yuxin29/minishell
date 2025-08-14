#include "minishell.h"

// examlpe of variable expandable in a redirection, and this variable is $?
// ls > output_$?.txt
// cat output_0.txt
// value = get_env_value_from_substr(input, start, var_len, envp);
//  need a spacial null check for this mem failure or ""
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
char	*expand_variables_in_str(t_exec_path *cmd, char *input, char **envp)
{
	int		i;
	int		var_len;

	i = 0;
	if (!input)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] == '"')
		{
			i += 2;
			continue ;
		}
		if (input[i] == '$' && ft_check_valid_var_name(input[i + 1]))
		{
			var_len = 0;
			while (ft_check_valid_var_name(input[i + 1 + var_len]))
				var_len++;
			return (replace_variable(cmd, input, i, envp));
		}
		else
			i++;
	}
	return (input);
}

void	expand_redirection(t_exec_path *cmd, t_cmd *cmd_list, char **envp)
{
	t_redir	*redir;
	char	*value;

	redir = cmd_list->redirections;
	while (redir)
	{
		if (redir->type == 5)
		{
			redir = redir->next;
			continue ;
		}
		if (redir->file && ft_strchr(redir->file, '$')
			&& should_expand(redir->file, 2))
		{
			value = expand_variables_in_str(cmd, redir->file, envp);
			if (!value)
				return ;
			if (value && value != redir->file)
			{
				free(redir->file);
				redir->file = value;
			}
		}
		redir = redir->next;
	}
}

void	expand_argv(t_exec_path *cmd, char **argv, int *quote_type, char **envp)
{
	int		i;
	char	*value;

	i = 0;
	while (argv && argv[i])
	{
		if ((!quote_type || should_expand(argv[i], quote_type[i]))
			&& ft_strchr(argv[i], '$'))
		{
			value = expand_variables_in_str(cmd, argv[i], envp);
			if (!value)
				continue ;
			if (value != argv[i])
			{
				free(argv[i]);
				argv[i] = value;
			}
		}
		i++;
	}
}

//call this after getting the cmd list
//go through all cmd and check all word without single quotes,
// not expanding heredocs
//after this Here I already got the updated cmd list
void	expand_all_cmds(t_exec_path *cmd, t_cmd *cmd_list, char **envp)
{
	while (cmd_list)
	{
		if (cmd_list->argv)
			expand_argv(cmd, cmd_list->argv, cmd_list->quote_type, envp);
		if (cmd_list->redirections)
			expand_redirection(cmd, cmd_list, envp);
		cmd_list = cmd_list->next;
	}
}
