#include "minishell.h"


void	split_env(char *str, char **key, char **value)
{
	char *eq;

	eq = ft_strchr(str, "=");
	if (!eq)
	{
		
	}
	*key = ft_strndup(eq, eq - str);
	*value = ft_strdup(eq + 1);
}
