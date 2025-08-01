#include "parsing.h"
#include "minishell.h"
#include "exec.h"

char	*expand_variables_in_str(char *input, char **envp)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1]
			&& (ft_isalpha(input[i + 1]) || input[i + 1] == '_'))
		{
			input = replace_variable_in_str(input, i, envp);
			i = 0;
		}
		else
			i++;
	}
	free(input);
	return (result);
}

//not corect yet, need to loop though all cmd->redirections
void	expand_redirection(t_cmd *cmd_list, char **envp)
{
	char	*value;

	while (cmd_list->redirections)
	{
		if (cmd_list->redirections->file && ft_strchr(cmd_list->redirections->file, '$'))
		{
			value = expand_variables_in_str(cmd_list->redirections->file, envp);
			if (value)
			{
				free(cmd_list->redirections->file);
				cmd_list->redirections->file = value;
			}
		}
		cmd_list->redirections = cmd_list->redirections->next;
	}
}

//call this after getting the cmd list
//go through all cmd and check all word without single quotes, 
// not expanding heredocs
//after this Here I already got the updated cmd list
void	expand_all_cmds(t_cmd *cmd_list, char **envp)
{
	int		i;
	char	*value;

	while (cmd_list)
	{
		i = 0;
		while (cmd_list->argv && cmd_list->argv[i])
		{
			if (ft_strchr(cmd_list->argv[i], '$'))
			{
				value = expand_variables_in_str(cmd_list->argv[i], envp);
				if (value)
					cmd_list->argv[i] = value;
			}
			i++;
		}
		if (cmd_list->redirections->file && ft_strchr(cmd_list->redirections->file, '$'))
			expand_redirection(cmd_list, envp);
		cmd_list = cmd_list->next;
	}
}
