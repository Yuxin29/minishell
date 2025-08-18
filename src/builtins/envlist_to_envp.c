#include "minishell.h"

static int	count_size(t_env *head)
{
	t_env	*cur;
	int		count;

	cur = head;
	count = 0;
	while (cur)
	{
		if (cur->exported && cur->value)
			count++;
		cur = cur->next;
	}
	return (count);
}

static char	*do_strjoin(char *key, char *value)
{
	char	*tmp_join;
	char	*result;

	if (!value)
		return (NULL);
	tmp_join = ft_strjoin(key, "=");
	if (!tmp_join)
		return (NULL);
	result = ft_strjoin(tmp_join, value);
	free(tmp_join);
	return (result);
}

static void	free_envp(char **envp, int i)
{
	while (--i >= 0)
		free(envp[i]);
	free(envp);
}

char	**env_list_to_envp(t_env *head)
{
	t_env	*cur;
	char	**envp;
	int		i;
	int		size;

	size = count_size(head);
	envp = malloc(sizeof(char *) * (size + 1));
	if (!envp)
		return (NULL);
	i = 0;
	cur = head;
	while (cur)
	{
		if (cur->exported && cur->value)
		{
			envp[i] = do_strjoin(cur->key, cur->value);
			if (!envp[i])
				return (free_envp(envp, i), NULL);
			i++;
		}
		cur = cur->next;
	}
	envp[i] = NULL;
	return (envp);
}

// used in pre expand
// return the value of a key from the envp
// special case: split[1] == NULL (e.g. FOO= in envp).
char	*get_env_value(char **envp, const char *key)
{
	int		j;
	char	**split;
	char	*new_value;

	j = -1;
	while (envp[++j])
	{
		split = ft_split(envp[j], '=');
		if (!split || !*split)
		{
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
