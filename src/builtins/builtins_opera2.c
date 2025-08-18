#include "minishell.h"

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
// no need to free target, from argv[1], mem from minishell argv
// buf is a local mem, path pointing to it, so need need to free path ever
// int	ft_cd(char **argv, t_env **env)
// {
// 	char	*path;
// 	char	buf[1024];
// 	char	*target;

// 	if (argv[2])
// 		return (errmsg_return_one("cd: too many arguments"));
// 	path = getcwd(buf, sizeof(buf));
// 	if (!path)
// 		return (perror_return_one("cd: getcwd (old)"));
// 	target = argv[1];
// 	if (!target)
// 		target = get_env(*env, "HOME");
// 	if (!target)
// 		return (errmsg_return_one("cd: HOME not set"));
// 	if (chdir(target) != 0)
// 		return (perror_return_one("cd"));
// 	set_env(env, "OLDPWD", path);
// 	path = getcwd(buf, sizeof(buf));
// 	if (!path)
// 		return (perror_return_one("cd: getcwd (new)"));
// 	set_env(env, "PWD", path);
// 	return (0);
// }
int	ft_cd(char **argv, t_env *env)
{
	char	oldpwd[4096];
	char	newpwd[4096];

	if (!argv[1])
	{
		fprintf(stderr, "cd: missing argument\n");
		return (1);
	}
	if (argv[2])
		return (errmsg_return_one("cd: too many arguments")); //modify 0818
	if (!getcwd(oldpwd, sizeof(oldpwd)))
		oldpwd[0] = '\0';
	if (chdir(argv[1]) != 0)
	{
		perror("cd");
		return (1);
	}
	if (getcwd(newpwd, sizeof(newpwd)))
	{
		set_env(&env, "OLDPWD", oldpwd);
		set_env(&env, "PWD", newpwd);
	}
	return (0);
}