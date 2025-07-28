#include "minishell.h"

static int	count_size(t_env *head)
{
	t_env *cur;
	int		count;

	cur = head;
	count = 0;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	return (count);
}

static char	*do_strjoin(char *key, char *value)
{
	char	*tmp_join;
	char	*result;

	tmp_join = ft_strjoin(key, "=");
	if (!tmp_join)
		return (0);
	result = ft_strjoin(tmp_join, value);
	free(tmp_join);
	return (result);
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
		envp[i] = do_strjoin(cur->key, cur->value);
		if (!envp[i])
			return (NULL);
		i++;
		cur = cur->next;
	}
	envp[i] = NULL;
	return (envp);
}
