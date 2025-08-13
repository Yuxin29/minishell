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

// Bash uses strtol() or strtoll() under the hood to parse the number.
// That means: long long (the range of a 64-bit signed integer)
// need for builtin exit
long long	ft_atoll(char *str)
{
	int			i;
	int			sign;
	long long	number;

	i = 0;
	sign = 1;
	number = 0;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		number = number * 10 + (str[i] - '0');
		i++;
	}
	return (number * sign);
}

// need for builtin exit
int ft_is_numeric(char *str)
{
	int	n;

	n = 0;
	if (str[0] == '-' || str[0] == '+')
		n++;
	if (!str[n])
		return (1);
	while (str[n])
	{
		if (!ft_isdigit(str[n]))
			return (1);
		n++;
	}
	if (str[0] == '-')
	{
		if (ft_strcmp(str, LLONG_MIN_STR) > 0)
			return (1); // error: too small
	}
	else
	{
		if (ft_strcmp(LLONG_MAX_STR, str) < 0) // I compare the string direclyt
			return (1);
	}
	return (0);
}
