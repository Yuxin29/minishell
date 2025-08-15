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
	new_node->exported = 1;
	new_node->next = *env;
	*env = new_node;
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

int env_count(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env -> next;
	}
	return (count);
}

void	sort_copy_list(t_env **copy_list, int size)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < size)
	{
		j = i + 1;
		while (j < size - 1)
		{
			if (ft_strcmp(copy_list[i]->key , copy_list[j]->key) > 0)
			{
				tmp = copy_list[i];
				copy_list[i] =  copy_list[j];
				copy_list[j] = tmp;
			}
			j++;
		}
		i++;
	}
}
