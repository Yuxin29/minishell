#include "minishell.h"

#include <readline/readline.h>
#include <readline/history.h>

// //yuxin debugging prints
// static void print_token_list(t_token *head)
// {
//     int i = 0;
//     if (!head)
//     {
//         printf("Tokens list is empty\n");
//         return;
//     }
//     while (head)
//     {
//         printf("Token[%d]: ", i++);
//         // Print token type as string inline
//         if (head->t_type == T_WORD)             printf("type=T_WORD (0), ");
//         else if (head->t_type == T_PIPE)       printf("type=T_PIPE (1), ");
//         else if (head->t_type == T_REDIRECT_IN)printf("type=T_REDIRECT_IN (2), ");
//         else if (head->t_type == T_REDIRECT_OUT)printf("type=T_REDIRECT_OUT (3), ");
//         else if (head->t_type == T_APPEND)     printf("type=T_APPEND (4), ");
//         else if (head->t_type == T_HEREDOC)    printf("type=T_HEREDOC (5), ");
//         else                                  printf("type=UNKNOWN (%d), ", head->t_type);

//         // Print quote type inline
//         if (head->quote_type == 0)              printf("quote=NONE (0), ");
//         else if (head->quote_type == 1)         printf("quote=SINGLE_QUOTE (1), ");
//         else if (head->quote_type == 2)         printf("quote=DOUBLE_QUOTE (2), ");
//         else                                   printf("quote=UNKNOWN (%d), ", head->quote_type);

//         printf("str='%s'\n", head->str ? head->str : "(null)");

//         head = head->next;
//     }
// }

// static void print_cmd_list(t_cmd *head)
// {
//     int cmd_i = 0;
//     if (!head)
//     {
//         printf("Commands list is empty\n");
//         return;
//     }

//     while (head)
//     {
//         printf("Command[%d]:\n", cmd_i++);

//         // Print argv array and corresponding quote_type array
//         if (!head->argv)
//         {
//             printf("  argv: (null)\n");
//         }
//         else
//         {
//             printf("  argv:\n");
//             for (int i = 0; head->argv[i] != NULL; i++)
//             {
//                 printf("    argv[%d]: '%s'", i, head->argv[i]);
//                 if (head->quote_type)
//                 {
//                     int q = head->quote_type[i];
//                     if (q == 0)      printf(" (quote=NONE)");
//                     else if (q == 1) printf(" (quote=SINGLE_QUOTE)");
//                     else if (q == 2) printf(" (quote=DOUBLE_QUOTE)");
//                     else             printf(" (quote=UNKNOWN %d)", q);
//                 }
//                 printf("\n");
//             }
//         }

//         // Print redirections
//         if (!head->redirections)
//         {
//             printf("  redirections: (none)\n");
//         }
//         else
//         {
//             printf("  redirections:\n");
//             t_redir *r = head->redirections;
//             int r_i = 0;
//             while (r)
//             {
//                 printf("    redir[%d]: ", r_i++);
//                 if (r->type == T_REDIRECT_IN)       printf("type=T_REDIRECT_IN (2), ");
//                 else if (r->type == T_REDIRECT_OUT) printf("type=T_REDIRECT_OUT (3), ");
//                 else if (r->type == T_APPEND)        printf("type=T_APPEND (4), ");
//                 else if (r->type == T_HEREDOC)       printf("type=T_HEREDOC (5), ");
//                 else                                printf("type=UNKNOWN (%d), ", r->type);

//                 printf("file='%s'", r->file ? r->file : "(null)");
//                 if (r->type == T_HEREDOC)
//                     printf(", heredoc_delim='%s'", r->heredoc_delim ? r->heredoc_delim : "(null)");

//                 printf("\n");
//                 r = r->next;
//             }
//         }

//         head = head->next;
//     }
// }


volatile sig_atomic_t	g_signal = 0;

static int	check_invalid_cmds(t_exec_path *exec_cmd, t_cmd *cmd_list)
{
	t_cmd	*cur;

	cur = cmd_list;
	while (cur)
	{
		if (!cur->argv || !cur->argv[0] || cur->argv[0][0] == '\0')
		{
			if (cur->redirections)
			{
				if (check_and_apply_redirections(cur) == -1)
					exec_cmd->exit_status = 1;
				else
					exec_cmd->exit_status = 0;
				return (1);
			}
			else
			{
				ft_putstr_fd("minishell: : command not found\n", 2);
				exec_cmd->exit_status = 127;
				return (1);
			}
		}
		cur = cur->next;
	}
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	t_env		*env_list;
	char		*line;
	char		*expanded_line;
	t_token		*token_list;
	t_exec_path	exec_cmd;
	t_cmd		*tmp;

	(void)argc;
	(void)argv;

	ft_memset(&exec_cmd, 0, sizeof(exec_cmd));
	rl_catch_signals = 0;
	signal_init();
	env_list = env_list_init(envp);
	if (!env_list)
	{
		ft_putstr_fd("Error: env list initialized failed\n", 2);
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break;
		}
		if(*line)
		{
			add_history(line);
			exec_cmd.envp = env_list_to_envp(env_list);
			if (!exec_cmd.envp)
			{
				free(line);
				free_env_list(env_list);
				ft_putstr_fd("Error: env list initialized failed\n", 2);
				exit(EXIT_FAILURE);
			}
			
			//yuxin added this part
			expanded_line = pre_expand_line(&exec_cmd, line, exec_cmd.envp); //null check this one
			free(line);
			if (!expanded_line)
			{
				ft_free_arr(exec_cmd.envp);
				free_env_list(env_list);
				ft_putstr_fd("Error: pre expanding failure\n", 2);
				exit(EXIT_FAILURE);
			}

			token_list = get_token_list(&exec_cmd, expanded_line); //covert line to token list
			free(expanded_line);
			if (!token_list)
			{
				ft_free_arr(exec_cmd.envp);
				if (exec_cmd.exit_status == 2)
					continue;
				else
				{
					free_env_list(env_list);
					ft_putstr_fd("Error: get token list failed from memory failure\n", 2);
					exit(EXIT_FAILURE);
				}
			}

			//yuxin debuging prints
			//print_token_list(token_list);

			exec_cmd.whole_cmd = build_command_list(&exec_cmd, token_list); //convert token list to command list
			free_token_list(token_list);
			if (!exec_cmd.whole_cmd || exec_cmd.exit_status == 2)
			{
				ft_free_arr(exec_cmd.envp);
				if (exec_cmd.exit_status == 2)
					continue;
				else if (exec_cmd.exit_status == 130)
					continue;
				else
				{
					ft_putstr_fd("check aaaa\n", 2);
					free_env_list(env_list);
					ft_putstr_fd("Error: build command list failed from memory failure\n", 2);
					exit(EXIT_FAILURE);
				}
			}
			
			//yuxin debiging prints
			//print_cmd_list(exec_cmd.whole_cmd);

			expand_all_cmds(&exec_cmd, exec_cmd.whole_cmd, exec_cmd.envp);

			if (check_invalid_cmds(&exec_cmd, exec_cmd.whole_cmd))
			{
				free_t_exec_path(&exec_cmd);
				continue;
			}

			tmp = exec_cmd.whole_cmd;
			while (tmp)
			{
				if (!is_builtin(tmp->argv[0]))
					tmp->cmd_path = get_cmd_path(tmp->argv[0], env_list);
				else
					tmp->cmd_path = NULL;
				tmp = tmp->next;
			}

			if (!exec_cmd.whole_cmd->next)
			{
				if (is_builtin(exec_cmd.whole_cmd->argv[0]))
					run_builtin_with_redir(&exec_cmd, &env_list);
				else
					execute_single_cmd(&exec_cmd);
			}
			else
				execute_pipeline(&exec_cmd, env_list);
			free_t_exec_path(&exec_cmd);
			continue;
		}
		rl_clear_history();
	}
	free_env_list(env_list);
	return (0);
}