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

// ft_export(char *argv, char **envp)
// {

// }

