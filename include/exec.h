#ifndef EXEC_H
# define EXEC_H

# include <sys/wait.h>
# include "parsing.h"
# include "minishell.h"

typedef struct s_cmd t_cmd;

typedef struct s_exec_path
{
	t_cmd	*whole_cmd;
	char	*cmd_path;
	char	**envp;
}	t_exec_path;

char	*get_cmd_path(char *cmd, char **envp);
int		execute_external_cmd(t_exec_path *cmd);

#endif
