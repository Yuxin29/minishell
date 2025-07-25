#include "minishell.h"

// int	main(void)
// {
// 	setup_signals();
// 	repl_loop();
// 	return (0);
// }

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
	t_exec_path exec_cmd;
	char	*line;
	t_token	*token_list;

	(void)argc;
	(void)argv;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line) //Ctrl+D
			break ;
		if(*line)
		{
			add_history(line); //no need to care about the history memory

			token_list = get_token_list(line);
			//check null
			free(line);

			exec_cmd.whole_cmd = build_command_list(token_list);
			//check null
			free_token_list(token_list);

			exec_cmd.envp = envp;
			exec_cmd.cmd_path = get_cmd_path(exec_cmd.whole_cmd->argv[0], exec_cmd.envp);
			if (!exec_cmd.cmd_path)
			{
				printf("minishell: %s : command not found\n", exec_cmd.whole_cmd->argv[0]);
				free_cmd_list(exec_cmd.whole_cmd);
				exit(127);
			}
			printf("%s\n", exec_cmd.cmd_path);
			free(exec_cmd.cmd_path);
			free_cmd_list(exec_cmd.whole_cmd);
		}
		return (0);
	}
}
