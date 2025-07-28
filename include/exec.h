#ifndef EXEC_H
# define EXEC_H

# include <sys/wait.h>

typedef struct s_exec_path
{
	t_cmd	*whole_cmd;
	char	*cmd_path;
	char	**envp;
}	t_exec_path;

typedef struct	s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

char	*get_cmd_path(char *cmd, char **envp);
int		execute_cmd(t_exec_path *cmd);

#endif
