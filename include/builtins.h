#ifndef BUILTINS_H
# define BUILTINS_H

# define PATH_MAX 4096

int	is_builtin(char *cmd);
int	execute_builtin(char **argv, char **envp);

int	ft_echo(char **argv);
int ft_cd(char **argv, char **envp);
int	ft_pwd(void);
// int	ft_export(char **argv, t_env **env);
// int	ft_unset(char **argv, t_env **env);
// int	ft_env(t_env *env);
//int	ft_exit(char **argv, t_env *env);

#endif
