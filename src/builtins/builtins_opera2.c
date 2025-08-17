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

// used in ft_cd
// cd: no argv, cd to HOME, I still have this even subject does not require
// cd -:  return to last folder and print out last folder, do we need to hand do this
// cd tatgert: change to target
// subject: cd with only a relative or absolute path
// static char *get_target_location(char **argv, t_env *env)
// {
// 	char *target;
	
// 	if (!argv[1])
// 	{
// 		target = get_env(env, "HOME");
// 		if (!target)
// 		{
// 			ft_putendl_fd("cd: HOME not set", 2);
// 			return (NULL);
// 		}
// 	}
// 	else if (strcmp(argv[1], "-") == 0)
// 	{
// 		target = get_env(env, "OLDPWD");
// 		if (!target)
// 		{
// 			ft_putendl_fd("cd: OLDPWD not set", 2);
// 			return (NULL);
// 		}
// 		printf("%s\n", target);
// 	}
// 	else
// 		target = argv[1];
// 	return (target);
// }

// NAME		chdir, - change working directory
// SYNOPSIS	int chdir(const char *path);
// On success, zero is returned.  On error, -1 is returned
// no need to free target, from argv[1], mem from minishell argv
int	ft_cd(char **argv, t_env *env)
{
	char	oldpwd[4096];
	char	newpwd[4096];

	if (!argv[1])
		return (errmsg_return_one("cd: missing argument\n"));
	if (argv[1] && argv[2])
		return (errmsg_return_one("cd: too many arguments\n"));
	if (!getcwd(oldpwd, sizeof(oldpwd)))
		oldpwd[0] = '\0';
	if (chdir(argv[1]) != 0)  // subject requirement: cd with only a relative or absolute path
		return (perror_return_one("cd"));
	if (getcwd(newpwd, sizeof(newpwd)))
	{
		set_env(&env, "OLDPWD", oldpwd);
		set_env(&env, "PWD", newpwd);
	}
	return (0);
}