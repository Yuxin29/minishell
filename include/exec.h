#ifndef EXEC_H
# define EXEC_H

typedef struct s_exec_path
{
	t_cmd	*whole_cmd;
	char	*cmd_path;
	char	**envp;
}	t_exec_path;

char	*get_cmd_path(char *cmd, char **envp);

#endif
