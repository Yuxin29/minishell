#include "minishell.h"

//below 2 need to be move to utils later
static char *error_and_return(const char *msg)
{
    perror(msg);
    return (NULL);
}

void	*safe_malloc(size_t size)
{
	void *ptr;

	ptr = malloc(size);
	if (!ptr)
		error_and_return("malloc failed\n");
	return (ptr);
}