#include "minishell.h"

void	set_env(t_env **env, char *key, char *value)
{
	t_env	*cur;
	t_env	*new_node;

	cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			free(cur->value);
			cur->value = ft_strdup(value);
			return ;
		}
		cur = cur->next;
	}
	// if no exit node, create a new one
	new_node = env_new_node(key, value);
	new_node->next = *env;
	*env = new_node;
}

static void	unset_env(t_env **env, char *key)
{
	t_env *cur;
	t_env *pre;

	cur = *env;
	pre = NULL;
	while(cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (!pre)
				*env = cur->next;
			else
				pre->next = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return ;
		}
		pre = cur;
		cur = cur->next;
	}
}

int	ft_export(char **argv, t_env **env)
{
	int		i;
	char	*tmp;
	char	*key;
	char	*value;

	i = 1;
	while (argv[i])
	{
		tmp = ft_strchr(argv[i], '=');
		if (tmp)
		{
			*tmp = '\0';
			key = argv[i];
			value = tmp + 1;
			set_env(env, key, value);
			*tmp = '=';
		}
		i++;
	}
	return (0);
}

int	ft_unset(char **argv, t_env **env)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		unset_env(env, argv[i]);
		i++;
	}
	return (0);
}
