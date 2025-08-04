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

int ft_cd(char **argv)
{
	if (!argv[1])
	{
		ft_putstr_fd("cd :missing arguments\n", 2);
		return (1);
	}
	if (chdir(argv[1]) != 0) //On success, zero is returned.  On error, -1 is returned
	{
		perror("cd");
		return (1);
	}
	//should updata pwd/oldpwd in envp later
	return (0);
}

// int	ft_exit(char **argv, t_env **env)
// {

// }

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
/*The getcwd() function copies an absolute pathname of the current working directory to the array pointed  to by buf, which is of length size

On  success,  these  functions return a pointer to a string containing the pathname of the current working directory.
In the case of getcwd() and getwd() this is the same value as buf.
On failure, these functions return NULL, and errno is set to indicate the error.  The contents of the  array  pointed
to by buf are undefined on error.
*/

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

