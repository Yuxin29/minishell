#include "minishell.h"

void	repl_loop(void)
{
	char	*line;

	while(1)
	{
		line = readline("minishell$ "); //should free line
		if (!line) //Ctrl+D
		{
			printf("exit\n");
			break ;
		}
		if(*line)
		{
			add_history(line); //no need to care about the history memory
			printf("You enter: %s\n", line);
			free(line);
		}
	}
}
