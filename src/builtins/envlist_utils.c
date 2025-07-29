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

void	unset_env(t_env **env, char *key)
{
	
}
