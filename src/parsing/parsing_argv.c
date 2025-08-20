#include "minishell.h"

// used in parse_argv, for malloc str of strs
// > followed by no file already sorted out in check_token_syntax
static int	count_argv(t_token *start)
{
	int	count;

	count = 0;
	while (start && start->t_type != 1)
	{
		if (start->t_type == 0)
			count++;
		else if (start->t_type >= 2 && start->t_type <= 5)
			start = start->next;
		start = start->next;
	}
	return (count);
}

//used for parse argv
static int	malloc_for_agrv(t_cmd *cmd, t_token *tokens)
{
	int	len;

	len = count_argv(tokens);
	cmd->argv = malloc(sizeof(char *) * (len + 1));
	if (!cmd->argv)
	{
		perror("malloc");
		return (-1);
	}
	cmd->quote_type = malloc(sizeof(int) * len);
	if (!cmd->quote_type)
	{
		ft_free_arr(cmd->argv);
		perror("malloc");
		return (-1);
	}
	return (len);
}

// handle normal word_tokens, string and strings
// tokens = tokens->next->next; safe becuase syntax already checked
t_token	*parse_argv(t_cmd *cmd, t_token *tokens)
{
	int	i;

	i = 0;
	if (malloc_for_agrv(cmd, tokens) == -1)
		return (NULL);
	while (tokens && tokens->t_type != 1)
	{
		if (tokens->t_type == 0)
		{
			cmd->argv[i] = ft_strndup(tokens->str, ft_strlen(tokens->str));
			if (!cmd->argv[i])
			{
				ft_free_arr(cmd->argv);
				free(cmd->quote_type);
				return ((t_token *)free_malloc_fail_null(NULL));
			}
			cmd->quote_type[i] = tokens->quote_type;
			i++;
			tokens = tokens->next;
		}
		else if (tokens->t_type >= 2 && tokens->t_type <= 5)
			tokens = tokens->next->next;
	}
	cmd->argv[i] = NULL;
	return (tokens);
}
