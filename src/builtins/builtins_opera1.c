#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0
				|| ft_strcmp(cmd, "cd") == 0
				|| ft_strcmp(cmd, "pwd") == 0
				|| ft_strcmp(cmd, "export") == 0
				|| ft_strcmp(cmd, "unset") == 0
				|| ft_strcmp(cmd, "env") == 0
				|| ft_strcmp(cmd, "exit") == 0);
}

int	execute_builtin_cmd(char **argv, t_env **env)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		return (ft_echo(argv));
	if (ft_strcmp(argv[0], "cd") == 0)
		return (ft_cd(argv));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (ft_pwd());
	//if (ft_strcmp(argv[0], "export") == 0)
		//return (ft_export(argv, envp));
	// if (ft_strcmp(argv[0], "unset") == 0)
	//	return (ft_unset(argv, envp);
	if (ft_strcmp(argv[0], "env") == 0)
		return (ft_env(*env));
	// if (ft_strcmp(argv[0], "exit") == 0)
	// 	return (ft_exit(argv, *envp));
	return (0);
}

int	ft_echo(char **argv)
{
	int	flag;
	int	i;

	flag = 0;
	i = 1;
	if (argv[i] && ft_strcmp(argv[1], "-n") == 0)
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

