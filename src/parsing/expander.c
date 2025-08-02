#include "parsing.h"
#include "minishell.h"
#include "exec.h"

extern int	g_exit_status;

// a bash examlpe of variable expandable in a redirection, and this variable is $?
// ls > output_$?.txt
// cat output_0.txt
char	*replace_variable_in_str(char *input, int pos, char **envp)
{
	int		start;
	int		var_len;
	char	*value;
	char	*prefix;
	char	*suffix;

	start = pos + 1;
	var_len = 1;
	if (input[pos + 1] == '?')
		value = ft_itoa(g_exit_status);
	else
	{
		var_len = 0;
		while (input[start + var_len] && (ft_isalnum(input[start + var_len]) || input[start + var_len] == '_'))
			var_len++;
		value = get_env_value_from_substr(input, start, var_len, envp);
		if (!value)
			return (free_malloc_fail_null(input));
	}
	prefix = ft_substr(input, 0, pos);
	suffix = ft_strdup(input + start + var_len);
	free(input);
	if (!prefix)
		return (free_malloc_fail_null(value));
	if (!suffix)
	{
		free(prefix);
		return (free_malloc_fail_null(value));
	}
	return (join_three_and_free(prefix, value, suffix));
}

char	*expand_variables_in_str(char *input, char **envp)
{
	int		i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] && ((input[i + 1] == '?'
			|| ft_isalpha(input[i + 1]) || input[i + 1] == '_')))
		{
			input = replace_variable_in_str(input, i, envp);
			if (!input)
				return (NULL);
			i = 0;
		}
		else
			i++;
	}
	return (input);
}

void	expand_redirection(t_cmd *cmd_list, char **envp)
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
		if (redir->file && ft_strchr(redir->file, '$'))
		{
			value = expand_variables_in_str(redir->file, envp);
			if (value)
			{
				free(redir->file);
				redir->file = value;
			}
		}
		redir = redir->next;
	}
}

void	expand_argv(char **argv, char **envp)
{
	int		i;
	char	*value;

	i = 0;
	while (argv && argv[i])
	{
		if (ft_strchr(argv[i], '$'))
		{
			value = expand_variables_in_str(argv[i], envp);
			if (value)
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
void	expand_all_cmds(t_cmd *cmd_list, char **envp)
{
	while (cmd_list)
	{
		if (cmd_list->argv)
			expand_argv(cmd_list->argv, envp);
		if (cmd_list->redirections)
			expand_redirection(cmd_list, envp);
		cmd_list = cmd_list->next;
	}
}
