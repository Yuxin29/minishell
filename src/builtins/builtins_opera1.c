#include "minishell.h"

int	ft_echo(char **argv)
{
	int	flag;
	int	i;

	flag = 0;
	i = 1;
	if (argv[i] && ft_strncmp(argv[1], "-n", 2) == 0)
	{
		flag = 1; //echo -n world
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (!flag)
		printf("\n");
	return (0);
}

// yuxin working on this now; I passed t_env **env into ft_cd
// NAME		chdir, - change working directory
// SYNOPSIS	int chdir(const char *path);
// On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.
int ft_cd(char **argv, t_env **env)
{
	char *old;
	char	*new;
	int		i;

	i = 0;
	if (!argv[1]) //yuxin added: yuwu@c2r6p9:~$ cd   it does not do anything, just new prompt
	{
		ft_putstr_fd("cd :missing arguments\n", 2);
		return (1);
	}
	old = ft_strdup(argc[1]); //nullcheck later
	if (chdir(argv[1]) != 0) //On success, zero is returned.  On error, -1 is returned
	{
		perror("cd");
		return (1);
	}
	while (env[i]) //should updata PWD/OLDPWD in envp later
	{
		if (ft_strcmp("OLDPWD", env->key == 0)) //I find old one
			//replace OLDPWD=xxx with old
			// I can use this directly: static void	set_env(t_env **env, char *key, char *value)
			set_env(t_env **env, "OLDPWD", old)
		if (ft_strcmp("PWD", env->key) == 0) 
			//replace OLDPWD=xxx with new
			set_env(t_env **env, "PWD", argv[1])
		env = env->next;
	}
	return (0);
}

// yuxin working on this
// in bash, if we exit, we exit bash into zsh shel
// so, this should be the same of control+D, exit the whle thing
// do we need to update signals?
// NAME		exit — cause the shell to exit
// SYNOPSIS	exit [n]
int	ft_exit(char **argv, t_env **env)
{
	int		i;
	int		status;

	i = 0;
	while (argv[i])
		i++;
	if (i != 2)
	{
		//perror("exit"); or this one
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	if (ft_isdigit(argv[1]))
	{
		ft_putstr_fd("minishell: exit: argv[1]: numeric argument required", 2);
		return (1);
	}
	status = ft_atoi(argv[1]);
	// deal with int max
	// if bigger than int max
	{
		ft_putstr_fd("minishell: exit: argv[1]: numeric argument required", 2);
		return (1);
	}
	exit(status)
	return (0);
}

// pwd
// The getcwd() function copies an absolute pathname of the current working directory to the array pointed  to by buf, 
// which is of length size
// On  success,  these  functions return a pointer to a string containing the pathname of the current working directory.
// In the case of getcwd() and getwd() this is the same value as buf.
// On failure, these functions return NULL, and errno is set to indicate the error.  The contents of the  array  pointed
// to by buf are undefined on error.
int	ft_pwd()
{
	char buf[PATH_MAX];

	if (getcwd(buf, sizeof(buf)))
	{
		printf("%s\n", buf);
		return (0);
	}
	perror("pwd");
	return (1);
}

//env
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

