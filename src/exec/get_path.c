#include "minishell.h"

static char	*find_path_in_envp(char **envp)
{
	char	*path_value;

	if(!envp)
		return (NULL);
	while(*envp) //get path
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0) //
		{
			path_value = *envp + 5;
			return (path_value);
		}
		envp++;
	}
	return (NULL);
}

static char	*match_cmd_in_path(char **paths, char *cmd)
{
	int		i;
	char	*joinslash;
	char	*com_path;

	i = 0;
	while(paths[i])
	{
		joinslash = ft_strjoin(paths[i], "/");
		if (!joinslash)
			return (NULL);
		com_path = ft_strjoin(joinslash, cmd);
		free(joinslash); //!!free it before return, because after return it can't free anymore
		if(!com_path)
			return (NULL);
		if(access(com_path, X_OK) == 0) //0 means ok, if it can be executed then free everything
			return (com_path);
		free(com_path);
		i++;
	}
	return (NULL);
}
char	*get_cmd_path(char *cmd, char **envp)
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
	path_value = find_path_in_envp(envp); //find "PATH=""
	if(!path_value)
		return (NULL);
	paths = ft_split(path_value, ':'); //split directory
	if(!paths)
		return (NULL);
	cmd_path = match_cmd_in_path(paths,cmd); //strjoin /+cmd, check by access
	ft_free_arr(paths);
	return (cmd_path); //do i need return ,becuse i've save it in exec_cmd
}

