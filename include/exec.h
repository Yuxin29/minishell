#ifndef EXEC_H
# define EXEC_H

# include <sys/wait.h>
# include "parsing.h"
# include "minishell.h"

typedef struct s_cmd t_cmd;

typedef struct s_exec_path
{
	t_cmd	*whole_cmd; //whole cmd_list
	char	*cmd_path; //eg. /bin/ls
	char	**envp;  //the copy one
}	t_exec_path;

//execute external cmd (which is different with builtin cmd)
char	*get_cmd_path(char *cmd, char **envp);
int		execute_external_cmd(t_exec_path *cmd);
//--------------------

//we check here if there are redirections symbles here in each cmd.
//we check a single cmd inside the child, once per cmd.
int		check_and_apply_redirections(t_cmd *cmd);

// return tmpfile, in token, check if t_type==5, then cmd_list->infile== tmp_file, then do redirections
char	*creat_heredoc_file(char *delim);

#endif
