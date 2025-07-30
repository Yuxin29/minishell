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

//redirections.c
/*after parsing, we got already cmd list.
we check here if there are redirections symbles here in each cmd.
there could be multiple heredocs within one line
so here we repeatedly check all the cmds in this linked list*/

void	check_and_apply_heredocs(t_cmd *cmd_list);

//we check here if there are redirections symbles here in each cmd.
//we check a single cmd inside the child, once per cmd.
int	check_and_apply_redirections(t_cmd *cmd);

#endif
