#include "minishell.h"

//for testing, delete later
static void	print_token_list(t_token *token_list)
{
	int i = 0;
	while (token_list)
	{
		printf("Token %d: %s (type=%d) (quoted=%d)\n", i, token_list->str, token_list->t_type, token_list->quote_type);
		token_list = token_list->next;
		i++;
	}
}

void	repl_loop(void)
{
	char	*line;
	t_token	*token_list;

	while(1)
	{
		line = readline("minishell$ "); //should free line
		if (!line) //Ctrl+D
			break ;
		if(*line)
		{
			add_history(line); //no need to care about the history memory
			token_list = get_token_list(line);
			//check null
			print_token_list(token_list);
			free_token_list(token_list);
			free(line);
		}
	}
}

// void	repl_loop(void)
// {
// 	char	*line;
// 	t_token	*token_list;
// 	//t_cmd	*cmd_list;
// 	t_exec_path *exec_cmd;

// 	while(1)
// 	{
// 		line = readline("minishell$ "); //should free line
// 		if (!line) //Ctrl+D
// 		{
// 			printf("exit\n");
// 			break ;
// 		}
// 		if(*line)
// 		{
// 			add_history(line); //no need to care about the history memory

// 			token_list = get_token_list(line);
// 			//check null
// 			free(line);

// 			exec_cmd->whole_cmd = build_command_list(token_list);
// 			//check null
// 			free_token_list(token_list);

// 			//printf("You enter: %s\n", line);
// 			//print_token_list(token_list);
// 			free_cmd_list(exec_cmd->whole_cmd);
// 		}
// 	}
// }
