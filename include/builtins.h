#ifndef BUILTINS_H
# define BUILTINS_H

# define PATH_MAX 4096

typedef struct s_cmd t_cmd;
typedef struct s_exec_path t_exec_path;

typedef struct	s_env
{
	char			*key;  //eg. USER
	char			*value; // eg. LinLiu
	int				exported;
	struct s_env	*next;
}	t_env;

//exec 7 builtin cmds
int		ft_cd(char **argv);
int		ft_echo(char **argv);
int		ft_pwd(void);
int		ft_env(t_env *env);
// int	ft_exit(char **argv, t_env *env);
int		ft_export(char **argv, t_env **env);
int		ft_unset(char **argv, t_env **env);
//-------------

/*in minishell, we would change the elements of the original env(impletment unset, export), so we have to copy the oringal env, and change them in copy version
first step is converting every envp[i] to a signal node, and connect these nodes to a linked list, in which we can add or delete env elements more easily than in array, after modifying the env list, we should convert linked list to array to do the following steps*/
t_env	*env_new_node(char *key, char *value);
t_env	*env_list_init(char **envp);
char	**env_list_to_envp(t_env *head);
void	free_env_list(t_env *head);

#endif
