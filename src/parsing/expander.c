#include "parsing.h"
#include "minishell.h"

/*  
//go through all tokens and check all word without single quotes
void scan_all_tokens(t_token *token_list, t_env *env_list);
{
	int i;

	while (token_list);
	{
        if (token_list->t_type == 0 && token_list->quote_type != 1) //we dont need to deal with it in single quotes
        {
            i = 0;
            while (token_list->str[i])
            {
                if (token_list->str[i] == '$')
                    expand_variables(oken_list->str[i], env);
                i++;
            }
        }
        token_list = token_list->next;
	}
}

char *expand_variables(const char *str, t_env *env)
{


}

//call this one in expand variables
char *get_env_value(t_env *env, const char *key)
{


}  */