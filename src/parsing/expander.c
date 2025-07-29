#include "parsing.h"
#include "minishell.h"
#include "exec.h"

static char	*expand_variables(char *var_name, char **envp)
{
	int		j;
	char	**split;
	char	*new_value;

	j = 0;
	while (envp[j])
	{
		split = ft_split(envp[j], '=');//null chck
		if (ft_strncmp(split[0], var_name, ft_strlen(split[0])) == 0)
		{
			new_value = ft_strdup(split[1]);
			ft_free_arr(split);
			return (new_value);
		}
		j++;
		ft_free_arr(split);
	}
	return (NULL);
}

static char	*change_value_part(char *str, int i, char *value)
{
	char	*before;
	char	*after;
	char	*temp;
	char	*result;
	int		var_len;

	var_len = 0;
	while (str[i + var_len]
        && ((ft_isalnum(str[i + var_len]) || str[i + var_len] == '_')))
		var_len++;
	before = ft_substr(str, 0, i - 1); // everything before the $
	after = ft_strdup(str + i + var_len); // after the variable name // need to malloc check
	temp = ft_strjoin(before, value); // need to malloc check
	result = ft_strjoin(temp, after); // need to malloc check
	free (temp);
	free (before);
	free (after);
	return (result);
}

/*
typedef struct s_exec_path
{
	t_cmd	*whole_cmd;
	char	*cmd_path;
	char	**envp;
}	t_exec_path;
exec_cmd.envp
*/
//call this after getting the tokens
//go through all tokens and check all word without single quotes
//Here I already got the updated, newest array of strings in the exec_cmd.envp
void	expand_all_tokens(t_token *token_list, t_exec_path exec_cmd)
{
	int		i;
	char	*value;
	char	*var_name;
	char	*new_str;

	while (token_list)
	{
		if (token_list->t_type == 0 && token_list->quote_type != 1)
		{
			i = 0;
			while (token_list->str[i])
			{
				if (token_list->str[i] == '$')
				{
					i++;
					var_name = ft_substr(token_list->str, i, ft_strlen(&token_list->str[i]));
					value = expand_variables(var_name, exec_cmd.envp);
					if (value)
					{
						new_str = change_value_part(token_list->str, i, value);
						free(token_list->str);
						token_list->str = new_str;
						free(value);
					}
				}
				i++;
			}
		}
		token_list = token_list->next;
	}
}
