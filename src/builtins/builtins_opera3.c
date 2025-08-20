#include "minishell.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	export_var(t_env **env, char *key)
{
	t_env	*node;
	t_env	*cur;

	cur = *env;
	node = NULL;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			node = cur;
			break ;
		}
		cur = cur->next;
	}
	if (node)
		node->exported = 1;
	else
	{
		node = env_new_node(key, NULL);
		if (!node)
			return ;
		node->next = *env;
		*env = node;
	}
}

static int	handle_export_arg(char *argv, t_env **env)
{
	char	*tmp;
	char	key[256];
	char	*value;

	tmp = ft_strchr(argv, '=');
	if (tmp)
		ft_strlcpy(key, argv, tmp - argv + 1);
	else
		ft_strlcpy(key, argv, sizeof(key));
	if (!is_valid_identifier(key))
	{
		print_error("export: ", argv, " : not a valid identifier");
		return (1);
	}
	if (tmp)
	{
		value = tmp + 1;
		set_env(env, key, value);
	}
	else
		export_var(env, key);
	return (0);
}

static void	print_export(t_env *env)
{
	int		size;
	t_env	**copy_list;
	int		i;

	size = env_count(env);
	copy_list = malloc(sizeof(t_env *) * size);
	if (!copy_list)
		return ;
	i = -1;
	while (++i < size)
	{
		copy_list[i] = env;
		env = env->next;
	}
	sort_copy_list(copy_list, size);
	i = -1;
	while (++i < size)
	{
		if (copy_list[i]->value == NULL)
			printf("declare -x %s\n", copy_list[i]->key);
		else
			printf("declare -x %s=\"%s\"\n", copy_list[i]->key,
				copy_list[i]->value);
	}
	free(copy_list);
}

int	ft_export(char **argv, t_env **env)
{
	int		i;
	int		ret;

	ret = 0;
	if (!argv[1])
	{
		print_export(*env);
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		if (handle_export_arg(argv[i], env))
			ret = 1;
		i++;
	}
	return (ret);
}
