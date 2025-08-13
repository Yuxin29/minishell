#ifndef BUILTINS_H
# define BUILTINS_H

# define PATH_MAX 4096

//yuxin added for exit
# define LLONG_MAX_STR "9223372036854775807"
# define LLONG_MIN_STR "9223372036854775808"

typedef struct s_cmd t_cmd;
typedef struct s_exec_path t_exec_path;

typedef struct	s_env
{
	char			*key;  //eg. USER
	char			*value; // eg. LinLiu
	int				exported;
	struct s_env	*next;
}	t_env;

//7 builtin cmds:
//builtins_opera1
int		ft_cd(char **argv, t_env **env);
int		ft_echo(char **argv);
int		ft_exit(char **argv, t_exec_path *exec_cmd);
//builtins_opera2
int		ft_pwd(void);
int		ft_env(t_env *env);
int		ft_unset(char **argv, t_env **env);
//builtins_opera3
int		ft_export(char **argv, t_env **env);

/*in minishell, we would change the elements of the original env(impletment unset, export), so we have to copy the oringal env, and change them in copy version
first step is converting every envp[i] to a signal node, and connect these nodes to a linked list, in which we can add or delete env elements more easily than in array, after modifying the env list, we should convert linked list to array*/

//get env list
t_env	*env_new_node(char *key, char *value);
t_env	*env_list_init(char **envp);
void	free_env_list(t_env *head);
//env list to envp
char	**env_list_to_envp(t_env *head);
//builtins_utils
void		set_env(t_env **env, char *key, char *value);
void		print_error(const char *arg);
t_env		*env_find(t_env *env, char *key);
long long	ft_atoll(char *str);
int			ft_is_numeric(char *str);

#endif
