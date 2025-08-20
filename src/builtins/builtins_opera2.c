#include "minishell.h"

int	ft_env(t_env **env)
{
	t_env	*cur;

	cur = *env;
	while (cur)
	{
		if (cur->key && cur->value != NULL)
			printf("%s=%s\n", cur->key, cur->value);
		cur = cur->next;
	}
	return (0);
}

static void	unset_env(t_env **env, char *key)
{
	t_env	*cur;
	t_env	*pre;

	cur = *env;
	pre = NULL;
	while (cur)
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

// NAME		chdir, - change working directory
// SYNOPSIS	int chdir(const char *path);
// On success, zero is returned.  On error, -1 is returned
static char	*set_target(char **argv, t_env **env)
{
	char	*target;

	if (!argv[1])
	{
		target = get_env(*env, "HOME");
		if (!target)
		{
			ft_putendl_fd("cd: HOME not set", 2);
			return (NULL);
		}
	}
	else
	{
		if (argv[2])
		{
			ft_putendl_fd("cd: too many arguments", 2);
			return (NULL);
		}
		target = argv[1];
	}
	return (target);
}

int	ft_cd(char **argv, t_env **env)
{
	char	oldpwd[4096];
	char	newpwd[4096];
	char	*target;

	target = set_target(argv, env);
	if (!target)
		return (1);
	if (!getcwd(oldpwd, sizeof(oldpwd)))
		oldpwd[0] = '\0';
	if (chdir(target) != 0)
		return (errmsg_return_nbr("cd", 1, 1));
	if (getcwd(newpwd, sizeof(newpwd)))
	{
		if (oldpwd[0])
			set_env(env, "OLDPWD", oldpwd);
		set_env(env, "PWD", newpwd);
	}
	else
	{
		unset_env(env, "PWD");
		return (errmsg_return_nbr(CD_ERR_MSG, 1, 0));
	}
	return (0);
}
