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
			if (!*split)
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

// a string operation utils.
// used in expander to join prefix, replaced variable and suffix
// free the sources within itself, still need to null check after using
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

// to deal with echo $"HOME", should not expand like this
// 1 for should expand, 0 for not expand
int	should_expand(const char *str, int quote_type)
{
	if (quote_type == 1)
		return (0);
	if (quote_type == 2 && str && str[0] == '$' && str[1] == '"')
		return (0);
	return (1);
}
