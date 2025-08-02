#include "parsing.h"
#include "minishell.h"
#include "exec.h"

//return (new_value); //no need for null check or need
// char	*get_env_value(char **envp, const char *key)
// {
// 	int		j;
// 	char	**split;
// 	char	*new_value;

// 	j = 0;
// 	while (envp[j])
// 	{
// 		split = ft_split(envp[j], '=');
// 		if (!split || !*split)
// 			return (NULL);
// 		if (ft_strcmp(split[0], (char *)key) == 0)
// 		{
// 			new_value = ft_strdup(split[1]);
// 			if (!new_value)
// 			{	
// 				ft_free_arr(split);
// 				return (free_malloc_fail_null(NULL));
// 			}
// 			ft_free_arr(split);
// 			return (new_value);
// 		}
// 		j++;
// 		ft_free_arr(split);
// 	}
// 	return (NULL);
// }