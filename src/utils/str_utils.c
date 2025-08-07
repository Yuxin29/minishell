#include "minishell.h"

//no need to null check in ft_strjoin, it will return null if failed
//but need to null check when calling ft_strjoin_free
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		joined = ft_strjoin("", s2);
	else if (!s2)
		joined = ft_strjoin(s1, "");
	else
		joined = ft_strjoin(s1, s2);
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	return (joined);
}

// need for builtin exit
long long ft_atoll(char *str)
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
	int			n;

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
