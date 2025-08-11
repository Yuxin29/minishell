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

// yuxin working on this now; I passed t_env **env into ft_cd
// NAME		chdir, - change working directory
// SYNOPSIS	int chdir(const char *path);
// On success, zero is returned.  On error, -1 is returned
// I can use this directly:  void	set_env(t_env **env, char *key, char *value)
// buf is a local mem, path pointing to it, so need need to free path ever
int	ft_cd(char **argv, t_env **env)
{
	char	*path;
	char	buf[1024];
	char	*target;

	if (argv[2])
	{
		ft_putendl_fd("cd: too many arguments", 2);
		return (1);
	}
	path = getcwd(buf, sizeof(buf));
	if (!path)
	{
		perror("cd: getcwd (old)");
		return (1);
	}
	target = argv[1];
	if (!target)
		target = get_env(*env, "HOME");
	if (!target)
	{
		ft_putendl_fd("cd: HOME not set", 2);
		return (1);
	}
	if (chdir(target) != 0)
	{
		perror("cd");
		return (1);
	}
	set_env(env, "OLDPWD", path);
	path = getcwd(buf, sizeof(buf));
	if (!path)
	{
		perror("cd: getcwd (new)");
		return (1);
	}
	set_env(env, "PWD", path);
	return (0);
}

// yuxin working on this
// in bash, if we exit, we exit bash into zsh shel
// so, this should be the same of control+D, exit the whle thing
// do we need to update signals? probably not
// NAME		exit — cause the shell to exit
// SYNOPSIS	exit [n]
// Bash uses strtol() or strtoll() under the hood to parse the number.
// That means: long long (the range of a 64-bit signed integer)
//if (ft_is_numeric(argv[1])) fatal error 255 / exit 2 ?
int	ft_exit(char **argv, t_exec_path *exec_cmd)
{
	long long	status;

	if (!argv[1])
		exit(exec_cmd->exit_status);
	if (ft_is_numeric(argv[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": numeric argument required", 2);
		exit (2);
	}
	if (argv[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		exit (1);
	}
	status = ft_atoll(argv[1]);
	exit((unsigned char)status);
}
// minishell process ends with a specific exit code,
// and the parent shell (bash/zsh) automatically stores
// that exit code in the special variable $?.
//in the terminal, echo $? to check the (unsigned char)status


