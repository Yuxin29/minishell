#include "parsing.h"
#include "minishell.h"
#include "exec.h"

//return (new_value); //no need for null check or need
char	*get_env_value(char **envp, const char *key)
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
			ft_free_arr(split);
			return (new_value);
		}
		j++;
		ft_free_arr(split);
	}
	return (NULL);
}

char	*replace_variable_in_str(char *input, int pos, char **envp)
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
	value = get_env_value(envp, var_name);
	free(var_name);
	joined = ft_substr(input, 0, pos);
	if (value)
	{
		tmp = ft_strjoin(joined, value);
		free(joined);
		joined = tmp;
		free(value);
	}
	suffix = ft_strdup(input + start + var_len);
	new_input = ft_strjoin(joined, suffix);
	free(joined);
	free(suffix);
	free(input);
	return (new_input);
}