#include "minishell.h"

// need to null check the return value, but not need to free source str
// ATTENTION: when use it, need to be sure s1 and s2 are malloced
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

// used in checking malloc strs or strdup, strjoin and so on
// perror included, and it can free one string
char	*free_malloc_fail_null(char	*str)
{
	if (str)
		free (str);
	perror("malloc");
	return (NULL);
}

// commone version of bash support only alpha and '_'
// when use, not need to check null, null check included
// QUESTION: alpha or upper case only
// RECHECK: it checks char instead of string
int	ft_check_valid_var_name(char c)
{
	if (!c)
		return (0);
	if (ft_isalpha(c))
		return (1);
	else if (c == '_')
		return (1);
	return (0);
}

// a string operation utils.
// used in expander to join prefix, replaced variable and suffix
// free the sources within itself, still need to null check after using
char	*join_three_and_free(char *s1, char *s2, char *s3)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin_free(s1, s2);
	if (!temp)
	{
		free(s3);
		return (NULL);
	}
	result = ft_strjoin_free(temp, s3);
	return (result);
}
