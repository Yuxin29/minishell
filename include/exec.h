#ifndef EXEC_H
# define EXEC_H

typedef struct s_cmd t_cmd;
typedef struct s_env t_env;

typedef struct s_exec_path
{
	t_cmd	*whole_cmd; //whole cmd_list
	char	**envp;  //the copy one
	int		exit_status;
}	t_exec_path;

// typedef struct s_pipe_ex
// {
// 	int		prev;
// 	int		pipefd[2];
// 	pid_t	last_pid;
// }	t_pipe_ex;

//execute builtins cmd with redir
int		is_builtin(char *cmd);
int		execute_builtin_cmd(char **argv, t_env **env, t_exec_path *exec_cmd);
void	run_builtin_with_redir(t_exec_path *exec_cmd, t_env **env_list);
//single
char	*get_env(t_env *env, char *key);
char	*get_cmd_path(char *cmd, t_env *env_list);
void	execute_single_cmd(t_exec_path *cmd);
void	print_error_and_exit(t_cmd *cmd);
void	precheck_path_or_exit(char *path);
//pipline
void	execute_pipeline(t_exec_path *exec_cmd, t_env *env_list);
//redir and heredoc
 int	check_and_apply_redirections(t_cmd *cmd);
char	*creat_heredoc_file(char *delim);


#endif
