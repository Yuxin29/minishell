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
	t_env	*new_node;

	node = env_find(*env, key);
	if (node)
		node->exported = 1;
	else
	{
		new_node = malloc(sizeof(t_env));
		new_node->key = ft_strdup(key);
		if (!new_node->key)
			return ;
		new_node->value = ft_strdup("");
		if (!new_node->value)
			return ;
		new_node->exported = 1;
		new_node->next = *env;
		*env = new_node;
	}
}

static void	print_export(t_env *env)
{
	while (env)
	{
		if (env->value)
			printf("declare -x %s=\"%s\"\n", env->key, env->value);
		else
			printf("declare -x %s\n", env->key);
		env = env->next;
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
		print_error(argv);
		return (1);
	}
	if (tmp)
	{
		value = tmp + 1;
		set_env(env, key, value);
	}
	else
		export_var(env, argv);
	return (0);
}

int	ft_export(char **argv, t_env **env)
{
	int		i;
	int		ret;

	ret = 0;
	if (!argv[1])
		print_export(*env);
	i = 1;
	while (argv[i])
	{
		if (handle_export_arg(argv[i], env))
			ret = 1;
		i++;
	}
	return (ret);
}
