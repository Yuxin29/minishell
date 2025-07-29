#include "parsing.h"
#include "minishell.h"

static char *expand_variables(char *, char **envp)
{
    int j;
    char **split;
    char    *new_value;

    j = 0;
    while (envp[j])
    {
        split = ft_split(envp, '=');
        //null chck
        if (ft_strncmp(split[0], strs, ft_strlen(split[0])) == 0)
        {
            new_value = ft_strdup(split[1]);
            ft_free_arr(split);
            return (new_value);
        }
        j++;
        ft_free_arr(split);
    }
    return (NULL);
}

/*
typedef struct s_exec_path
{
	t_cmd	*whole_cmd;
	char	*cmd_path;
	char	**envp;
}	t_exec_path;
exec_cmd.envp
*/
//call this after getting the tokens
//go through all tokens and check all word without single quotes
//Here I already got the updated, newest array of strings in the exec_cmd.envp
void expand_all_tokens(t_token *token_list, t_exec_path exec_cmd);
{
	int i;
    char *value;

	while (token_list)
	{
        if (token_list->t_type == 0 && token_list->quote_type != 1) //we dont need to deal with it in single quotes
        {
            i = 0;
            while (token_list->str[i])
            {
                if (token_list->str[i] == '$')
                {
                    i++;
                    value = expand_variables(token_list->str[i], exec_cmd.envp);
                    if (value)
                        token_list->str[i] = ft_strdup(value);
                }
                i++;
            }
        }
        token_list = token_list->next;
	}
}
