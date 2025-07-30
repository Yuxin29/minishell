#include "minishell.h"

int	ft_env(t_env *env)
{
	while (env)
	{
		if (env->key && env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
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

// int	ft_exit(char **argv, t_env **env)
// {

// }
