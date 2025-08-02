#include "minishell.h"

//why is this necessary
//it is probably not needed, delete later
void	check_strndup(char *str, t_cmd *cmd, t_token *tokens)
{
	if (!str)
	{
		free_token_list(tokens);
		free_cmd_list(cmd);
		error_and_return("malloc failed", NULL);//free_t_errmsg_exit
	}
}

//no need to null check in ft_strjoin, it will return null if failed
//but need to null check when calling ft_strjoin_free
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	if (!s1)
		joined = ft_strjoin("", s2);
	else
		joined = ft_strjoin(s1, s2);
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	return (joined);
}