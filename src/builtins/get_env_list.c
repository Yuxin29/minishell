#include "minishell.h"

static void	split_env(char *str, char **key, char **value)
{
	char *eq;

	eq = ft_strchr(str, '=');
	if (!eq)
	{
		*key = ft_strdup(str);
		if (!*key)
			return ;
		*value = ft_strdup(""); // FOO
		if (!*value)
		{
			free(*key);
			return ;
		}
		return ;
	}
	*key = ft_strndup(str, eq - str);
	if (!*key)
		return ;
	*value = ft_strdup(eq + 1);
	if (!*value)
	{
		free(*key);
		return ;
	}
}

t_env	*env_new_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->exported = 0;
	node->key = ft_strdup(key);
	if (!node->key)
		return (free(node), NULL);
	node->value = ft_strdup(value);
	if (!node->value)
	{
		free(node->key);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

static t_env	*split_envp_create_node(char *env_str)
{
	t_env	*new_node;
	char	*key;
	char	*value;

	split_env(env_str, &key, &value);
	new_node = env_new_node(key, value);
	free(key);
	free(value);
	return (new_node);
}

t_env	*env_list_init(char **envp)
{
	t_env	*head;
	t_env	*tail;
	t_env	*new_node;
	int		i;

	i = 0;
	head = NULL;
	tail = NULL;
	while (envp[i])
	{
		new_node = split_envp_create_node(envp[i]);
		if (!new_node)
			return (free_env_list(head), NULL);
		if (!head)
			head = new_node;
		else
			tail->next = new_node;
		tail = new_node;
		i++;
	}
	return (head);
}

void	free_env_list(t_env *head)
{
	t_env	*cur;
	t_env	*next;

	if (!head)
		return ;
	cur = head;
	while(cur)
	{
		next = cur->next;
		free(cur->key);
		free(cur->value);
		free(cur);
		cur = next;
	}
}
