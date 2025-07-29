#include "parsing.h"
#include "minishell.h"

//
static char *get_var_name(t_token *token, int i)
{
    char *name;

    name = ft_strndup(token_list->str[i], ft_strlen(token->str - 1));
    if (!name)
    {
        perror("malloc");
        exit (1);
    }
    return (name);
}

//call this after getting the to
static char *expand_variables(t_token *token, char *name, char **envp)
{
    int j;
    char       *value;
    char        *key;
    char        *new_value;

    j = 0;
    while (envp[j])
    {
        value = ft_strchr(envp[i], '='); //null // malloc check
        key = ft_strncpy(envp, ft_strlen(envp[j]) - ft_strlen(key)); //null / malloc check
        if (ft_strncmp(key, ft_strlen(key), name) == 0)
            new_value = ft_substr();
        j++;
        free (value);
        free (key);
    }
    return (new_value);
}

//call this after getting the tokens
//go through all tokens and check all word without single quotes
void scan_all_tokens(t_token *token_list, t_env *env_list)
{
	int i;
    char *name

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
                    name = get_var_name(token_list, i);
                    token_list->str = expand_variables(token_list, name, env);
                }
                i++;
            }
        }
        token_list = token_list->next;
	}
}
