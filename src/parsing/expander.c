#include "parsing.h"
#include "minishell.h"
#include "exec.h"

//return (new_value); //no need for null check or need
static char	*get_env_value(char **envp, const char *key)
{
	int		j;
	char	**split;
	char	*new_value;

	j = 0;
	while (envp[j])
	{
		split = ft_split(envp[j], '=');
		if (!split || !*split)
			return (NULL);
		if (ft_strcmp(split[0], (char *)key) == 0)
		{
			new_value = ft_strdup(split[1]);
			if (!new_value)
				return (NULL);
			ft_free_arr(split);
			return (new_value);
		}
		j++;
		ft_free_arr(split);
	}
	return (NULL);
}

static char	*replace_variable_in_str(char *input, int pos, char **envp)
{
	char	*var_name;
	char	*value;
	char	*joined;
	char	*new_input;
	char	*suffix;
	char	*tmp;
	int		start;
	int		var_len;

	start = pos + 1;
	var_len = 0;
	while (input[start + var_len] && (ft_isalnum(input[start + var_len])
		|| input[start + var_len] == '_'))
		var_len++;
	var_name = ft_substr(input, start, var_len);
	if (!var_name)
		return (NULL);
	value = get_env_value(envp, var_name);
	free(var_name);
	if (!value)
		return (NULL);
	joined = ft_substr(input, 0, pos);//null check
	if (value)
	{
		tmp = ft_strjoin(joined, value);//null check
		free(joined);
		joined = tmp;
		free(value);
	}
	suffix = ft_strdup(input + start + var_len);//null check
	new_input = ft_strjoin(joined, suffix);//null check
	free(joined);
	free(suffix);
	free(input);
	return (new_input);
}

char	*expand_variables_in_str(char *input, char **envp)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1]
			&& (ft_isalpha(input[i + 1]) || input[i + 1] == '_'))
		{
			input = replace_variable_in_str(input, i, envp);
			if (!input)
				return(NULL);
			i = 0;
		}
		else
			i++;
	}
	free(input);
	return (result);
}

//not corect yet, need to loop though all cmd->redirections
void	expand_redirection(t_cmd *cmd_list, char **envp)
{
	char	*value;

	while (cmd_list->redirections)
	{
		if (cmd_list->redirections->file && ft_strchr(cmd_list->redirections->file, '$'))
		{
			value = expand_variables_in_str(cmd_list->redirections->file, envp);//null check
			if (value)
			{
				free(cmd_list->redirections->file);
				cmd_list->redirections->file = value;
			}
		}
		cmd_list->redirections = cmd_list->redirections->next;
	}
}

//call this after getting the cmd list
//go through all cmd and check all word without single quotes, 
// not expanding heredocs
//after this Here I already got the updated cmd list
void	expand_all_cmds(t_cmd *cmd_list, char **envp)
{
	int		i;
	char	*value;

	while (cmd_list)
	{
		i = 0;
		while (cmd_list->argv && cmd_list->argv[i])
		{
			if (ft_strchr(cmd_list->argv[i], '$'))
			{
				value = expand_variables_in_str(cmd_list->argv[i], envp);//null check
				if (value)
					cmd_list->argv[i] = value;
			}
			i++;
		}
		if (cmd_list->redirections->file && ft_strchr(cmd_list->redirections->file, '$'))
			expand_redirection(cmd_list, envp);
		cmd_list = cmd_list->next;
	}
}
