#include "parsing.h"
#include "minishell.h"
#include "exec.h"

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
			return (free_malloc_fail_null(NULL));
		if (ft_strcmp(split[0], (char *)key) == 0)
		{
			new_value = ft_strdup(split[1]);
			if (!new_value)
			{	
				ft_free_arr(split);
				return (free_malloc_fail_null(NULL));
			}
			ft_free_arr(split);
			return (new_value);
		}
		j++;
		ft_free_arr(split);
	}
	return (NULL);
}

char	*get_env_value_from_substr(char *input, int start, int var_len, char **envp)
{
	char	*var_name;
	char	*value;

	var_name = ft_substr(input, start, var_len);
	if (!var_name)
		return (NULL);
	value = get_env_value(envp, var_name);
	free(var_name);
	return (value);
}

char	*join_three_and_free(char *s1, char *s2, char *s3)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin_free(s1, s2);
	if (!temp)
	{
		free(s3);
		return (NULL);
	}
	result = ft_strjoin_free(temp, s3);
	return (result);
}