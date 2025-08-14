#include "minishell.h"
//# include "parsing.h"
//# include "utils.h"

//no need to null check in ft_strjoin, it will return null if failed
//but need to null check when calling ft_strjoin_free
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		joined = ft_strjoin("", s2);
	else if (!s2)
		joined = ft_strjoin(s1, "");
	else
		joined = ft_strjoin(s1, s2);
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	return (joined);
}

void	errmsg_set_status(char *msg, t_exec_path *cmd)
{
	if (msg)
		ft_putendl_fd(msg, 2);
	cmd->exit_status = 2;
}

//used in checking malloc strs or strdup, strjoin and so on
char	*free_malloc_fail_null(char	*str)
{
	if (str)
		free (str);
	perror("malloc: ");
	return (NULL);
}

int	ft_check_valid_var_name(char c)
{
	if (ft_isalpha(c))
		return (1);
	else if (c == '_')
		return (1);
	return (0);
}

void	print_error(const char *arg)
{
	ft_putstr_fd("export: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(" : not a valid identifier\n", 2);
}
