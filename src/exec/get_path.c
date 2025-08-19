#include "minishell.h"

char	*get_env(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static char	*match_cmd_in_path(char **paths, char *cmd, t_exec_path *exec_cmd)
{
	int		i;
	char	*joinslash;
	char	*com_path;

	i = 0;
	while (paths[i])
	{
		joinslash = ft_strjoin(paths[i], "/");
		if (!joinslash)
			return (free_malloc_fail_null_status(NULL, exec_cmd));
		com_path = ft_strjoin(joinslash, cmd);
		free(joinslash);
		if (!com_path)
			return (free_malloc_fail_null_status(NULL, exec_cmd));
		if (access(com_path, X_OK) == 0)
			return (com_path);
		free(com_path);
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd, t_env *env_list, t_exec_path *exec_cmd)
{
	char	**paths;
	char	*path_value;
	char	*cmd_path;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	path_value = get_env(env_list, "PATH");
	if (!path_value)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exec_cmd->exit_status = 127;
		return (NULL);
	}
	paths = ft_split(path_value, ':');
	if (!paths)
		return (free_malloc_fail_null_status(NULL, exec_cmd));
	cmd_path = match_cmd_in_path(paths, cmd, exec_cmd);
	ft_free_arr(paths);
	return (cmd_path);
}
