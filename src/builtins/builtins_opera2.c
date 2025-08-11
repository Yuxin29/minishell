#include "minishell.h"

static void	unset_env(t_env **env, char *key)
{
	t_env *cur;
	t_env *pre;

	cur = *env;
	pre = NULL;
	while(cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (!pre)
				*env = cur->next;
			else
				pre->next = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return ;
		}
		pre = cur;
		cur = cur->next;
	}
}

// pwd
// The getcwd() function copies an absolute pathname
// of the current working directory
// to the array pointed to by buf,
// which is of length size
// On success,  these functions return a pointer to a string
// containing the pathname of the current working directory.
// In the case of getcwd() and getwd() this is the same value as buf.
// On failure, these functions return NULL, and errno is set.
// The contents of the  array  pointed to by buf are undefined on error.
int	ft_pwd()
{
	char	buf[PATH_MAX];

	if (getcwd(buf, sizeof(buf)))
	{
		printf("%s\n", buf);
		return (0);
	}
	perror("pwd");
	return (1);
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
