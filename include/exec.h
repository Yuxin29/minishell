#ifndef EXEC_H
# define EXEC_H

# include <sys/wait.h>
# include <errno.h> //？？
// # include "parsing.h"
// # include "minishell.h"

typedef struct s_cmd t_cmd;
typedef struct s_env t_env;

typedef struct s_exec_path
{
	t_cmd	*whole_cmd; //whole cmd_list
	char	**envp;  //the copy one
	int		exit_status;
}	t_exec_path;

//execute builtins cmd with redir
int		is_builtin(char *cmd);
int		execute_builtin_cmd(char **argv, t_env **env);
//void	run_builtin_with_redir(t_cmd *cmd, t_env **env_list);
void	run_builtin_with_redir(t_exec_path *exec_cmd, t_env **env_list);
//------------------

//single
char	*get_cmd_path(char *cmd, t_env *env_list);
void	execute_single_cmd(t_exec_path *cmd);
void	print_error_and_exit(t_cmd *cmd);
//pipline
void	execute_pipeline(t_exec_path *exec_cmd, t_env *env_list);
//--------------------

 int	check_and_apply_redirections(t_cmd *cmd);
// return tmpfile, in parsing, check if t_type==5, then cmd_list->infile== tmp_file, then do redirections
char	*creat_heredoc_file(char *delim);


#endif
