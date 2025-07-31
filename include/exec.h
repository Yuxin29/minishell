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
	char	*cmd_path; //eg. /bin/ls
	char	**envp;  //the copy one
}	t_exec_path;

//execute builtins cmd
int		is_builtin(char *cmd);
int		execute_builtin_cmd(char **argv, t_env **env);
//run builtin with redir
void	run_builtin_with_redir(t_cmd *cmd, t_env **env_list);
//------------------

//execute external cmd (which is different with builtin cmd)
char	*get_cmd_path(char *cmd, char **envp);
void	execute_external_cmd(t_exec_path *cmd);
//--------------------

//we check here if there are redirections symbles here in each cmd.
//we check a single cmd inside the child, once per cmd.
int		check_and_apply_redirections(t_cmd *cmd);
// return tmpfile, in token, check if t_type==5, then cmd_list->infile== tmp_file, then do redirections
char	*creat_heredoc_file(char *delim);

#endif
