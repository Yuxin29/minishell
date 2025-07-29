#ifndef BUILTINS_H
# define BUILTINS_H

# define PATH_MAX 4096

typedef struct	s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

int		is_builtin(char *cmd);
int		execute_builtin_cmd(char **argv, t_env **env);


int		ft_cd(char **argv);
int		ft_echo(char **argv); //ok
int		ft_pwd(void); //ok
int		ft_env(t_env *env);
// int	ft_exit(char **argv, t_env *env);
// int	ft_export(char **argv, t_env **env);
// int	ft_unset(char **argv, t_env **env);

char	**env_list_to_envp(t_env *head);
t_env	*env_list_init(char **envp);
void	free_env_list(t_env *head);

// void	set_env(t_env **env, char *key, char *value);
// void	unset_env(t_env **env, char *key, char *value)

#endif
