#ifndef BUILTINS_H
# define BUILTINS_H

# define PATH_MAX 4096

typedef struct	s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

// typedef struct	s_env_list
// {
// 	t_env_node	*head;
// 	t_env_node	*tail;
// 	int			size;
// }	t_env_list;

int	is_builtin(char *cmd);
int	execute_builtin(char **argv, char **envp);

int	ft_echo(char **argv);
int	ft_cd(char **argv, char **envp);
int	ft_pwd(void);
// int	ft_export(char **argv, t_env **env);
// int	ft_unset(char **argv, t_env **env);
// int	ft_env(t_env *env);
// int	ft_exit(char **argv, t_env *env);

#endif
