#include "minishell.h"

//functions are are all called only before parsing and during parsing
// only necessary to free tokens and cmds
void	errmsg_set_status(char *msg, int	g_exit_status)
{
	if (msg)
		ft_putendl_fd(msg, 2);
	g_exit_status = 2;
}

char	*free_malloc_fail_null(char	*str)
{
	if (str)
		free (str);
	perror("malloc: ");
	return (NULL);
}

