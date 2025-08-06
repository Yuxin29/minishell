#include "minishell.h"

//functions are are all called only before parsing and during parsing
// used in pre leving/pre parsign checkings
void	errmsg_set_status(char *msg)
{
	if (msg)
		ft_putendl_fd(msg, 2);
	//g_exit_status = 2;
}

//used in checking malloc strs or strdup, strjoin and so on
char	*free_malloc_fail_null(char	*str)
{
	if (str)
		free (str);
	perror("malloc: ");
	return (NULL);
}

