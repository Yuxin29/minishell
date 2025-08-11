#include "minishell.h"

void	print_error(const char *arg)
{
	ft_putstr_fd("export: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(" : not a valid identifier\n", 2);
}

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

t_env	*env_find(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}
