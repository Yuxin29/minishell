#include "minishell.h"

static int	is_n_flag(char *s)
{
	int	i;

	if (ft_strncmp(s, "-n", 2) != 0)
		return (0);
	i = 2;
	while (s[i] == 'n')
		i++;
	if (s[i] != '\0')
		return (0);
	return (1);
}

int	ft_echo(char **argv)
{
	int	flag;
	int	i;

	flag = 0;
	i = 1;
	while (argv[i] && is_n_flag(argv[i]))
	{
		flag = 1;
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

// in bash, if we exit, we exit bash into zsh shel
// do we need to update signals? probably not
// exit — cause the shell to exit:	exit [n]
// if (ft_is_numeric(argv[1])) fatal error 255 / exit 2 ?
// minishell process ends with a specific exit code,
// and the parent shell (bash/zsh) automatically stores
// that exit code in the special variable $?.
//in the terminal, echo $? to check the (unsigned char)status

int	ft_exit(char **argv, t_exec_path *exec_cmd, t_env **env_list)
{
	long long	status;

	rl_clear_history();
	if (!argv[1])
	{
		free_t_exec_path(exec_cmd);
		free_env_list(*env_list);
		exit(exec_cmd->exit_status);
	}
	if (ft_is_numeric(argv[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": numeric argument required", 2);
		free_env_list(*env_list);
		free_t_exec_path(exec_cmd);
		exit (2);
	}
	if (argv[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		free_env_list(*env_list);
		free_t_exec_path(exec_cmd);
		exit (1);
	}
	status = ft_atoll(argv[1]);
	free_env_list(*env_list);
	free_t_exec_path(exec_cmd);
	exit((unsigned char)status);
}

// int	ft_exit(char **argv, t_exec_path *exec_cmd, t_env **env_list)
// {
// 	int status;

// 	if (argv[1])
// 		return (errmsg_return_one("exit with options not
//					required in minishell"));
// 	else
// 	{
// 		status = exec_cmd->exit_status;
// 		free_env_list(*env_list);
// 		free_t_exec_path(exec_cmd);
// 		exit(status);
// 		return (0);
// 	}
// }
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

int	ft_pwd(void)
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

int	ft_env(t_env *env)
{
	while (env)
	{
		if (env->key && env->value != NULL)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}
