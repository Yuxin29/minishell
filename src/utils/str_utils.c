#include "minishell.h"

//why is this necessary
//it is probably not needed, delete later
char	*check_strdup(char *dup, char *original)
{
	if (!dup)
	{
		free(original);
		perror("strdup");
	}
	return (NULL);
}

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

int	hd_is_interrupted(void)
{
	if (g_signal)
	{
		g_signal = 0;
		return (1);
	}
	return (0);
}
