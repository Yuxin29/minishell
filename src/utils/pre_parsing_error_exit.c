#include "minishell.h"

//functions are are all called only before parsing and during parsing
// only necessary to free tokens and cmds
void	errmsg_exit(char *msg, int status)
{
	if (msg)
		ft_putstr_fd(msg, 2);
	printf("🔴 Exit Status: %d\n", status); //delete later
	longjmp(g_jmpbuf, 1);//delete later
	exit(status);
}

void	free_errmsg_exit(t_token *tokens, char *msg, int status)
{
	if (tokens)
		free_token_list(tokens);
	if (msg)
		ft_putstr_fd(msg, 2);
	printf("🔴 Exit Status: %d\n", status);//delete later
	longjmp(g_jmpbuf, 1);//delete later
	exit (status);
}

void	free_tc_errmsg_exit(t_token *tokens, t_cmd *cmds, char *msg, int status)
{
	if (tokens)
		free_token_list(tokens);
	if (cmds)
		free_cmd_list(cmds);
	if (msg)
		ft_putstr_fd(msg, 2);
	printf("🔴 Exit Status: %d\n", status);//delete later
	longjmp(g_jmpbuf, 1);//delete later
	exit (status);
}
