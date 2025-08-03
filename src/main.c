#include "minishell.h"

int	g_exit_status = 0;

//lin newster main
// int main(int argc, char **argv, char **envp)
// {
// 	t_env		*env_list;
// 	char		*line;
// 	t_token		*token_list;
// 	t_exec_path	exec_cmd;

// 	(void)argc;
// 	(void)argv;

    // yuxin added
    // rl_catch_signals = 0;   // forbidden readline default signalsm global variables from the readline.h
	// setup_signals();      
// 	ft_memset(&exec_cmd, 0, sizeof(exec_cmd));
// 	env_list = env_list_init(envp);
// 	if (!env_list)
// 	{
// 		ft_putstr_fd("Error: env list initialized failed\n", 2);
// 		exit(EXIT_FAILURE);
// 	}
// 	while (1)
// 	{
// 		line = readline("minishell$ ");
// 		if (!line)
// 			break ;
// 		if(*line)
// 		{
// 			add_history(line);
// 			//convert list to envp array
// 			exec_cmd.envp = env_list_to_envp(env_list);
// 			if (!exec_cmd.envp)
// 			{
// 				free(line);
// 				free_env_list(env_list);
// 				ft_putstr_fd("Error: env list initialized failed\n", 2);
// 				exit(EXIT_FAILURE);
// 			}
// 			//convert line token list
// 			token_list = get_token_list(line);
// 			free(line);
// 			if (!token_list)
// 			{
// 				free_env_list(env_list);
// 				ft_free_arr(exec_cmd.envp);
// 				ft_putstr_fd("Error: get token list failed\n", 2);
// 				exit(EXIT_FAILURE);
// 			}
//             //print tokens;
//             //print_token_list(token_list);

// 			//convert token list to command list
// 			exec_cmd.whole_cmd = build_command_list(token_list);
// 			free_token_list(token_list);
//             //print cmd
//             //print_cmd_list(exec_cmd.whole_cmd);
// 			if (!exec_cmd.whole_cmd)
// 			{
// 				free_env_list(env_list);
// 				ft_free_arr(exec_cmd.envp);
// 				ft_putstr_fd("Error: build command list failed\n", 2);
//                 free_t_exec_path(&exec_cmd);
// 				exit(EXIT_FAILURE);
// 			}
//            expand_all_cmds(exec_cmd.whole_cmd, exec_cmd.envp);
// 			//check < infile
// 			if (!exec_cmd.whole_cmd->argv || !exec_cmd.whole_cmd->argv[0])
// 			{
// 				if (check_and_apply_redirections(exec_cmd.whole_cmd) == -1)
// 					g_exit_status = 1;
// 				else
// 					g_exit_status = 0;
// 				free_t_exec_path(&exec_cmd);
// 				continue;
// 			}
// 			//check if null or empty
// 			if (exec_cmd.whole_cmd->argv[0][0] == '\0')
// 			{
// 				ft_putstr_fd("minishell: : command not found\n", 2);
// 				g_exit_status = 127;
// 				free_t_exec_path(&exec_cmd);
// 				continue;
// 			}
// 			//if bulitin, no need to find cmd_path, just execute(need to deal with other things in it)
// 			if (is_builtin(exec_cmd.whole_cmd->argv[0]))
// 			{
// 				exec_cmd.cmd_path = NULL;
// 				run_builtin_with_redir(exec_cmd.whole_cmd, &env_list);
// 				free_t_exec_path(&exec_cmd);
// 				continue;
// 			}
// 			//if internal cmd, get cmd_path first, then run external cmd
// 			else
// 			{
// 				exec_cmd.cmd_path = get_cmd_path(exec_cmd.whole_cmd->argv[0], exec_cmd.envp);
// 				if (!exec_cmd.cmd_path)
// 				{
// 					ft_putstr_fd(exec_cmd.whole_cmd->argv[0], 2);
// 					ft_putstr_fd(": command not found\n", 2);
// 					g_exit_status = 127;
// 					free_t_exec_path(&exec_cmd);
// 					continue;
// 				}
// 				execute_external_cmd(&exec_cmd);
// 				free_t_exec_path(&exec_cmd);
// 				continue;
// 			}
// 		}
// 		//free_t_exec_path(&exec_cmd);
// 		rl_clear_history();
// 	}
// 	free_env_list(env_list);
// 	return (0);
// }

// #include <setjmp.h> //delete later
// jmp_buf g_jmpbuf;//delete later

char *test_lines[] =
{

    // ✅ Valid test cases
    // Test 1: Simple Command
    "echo hello world",

        // Test  continuous redirections
    "cat << infile1 << infile2 << infile3 >> out4",

    // Test 2: Pipe and Redirection
    "cat < file | grep foo > out.txt",
    
    // Test 2: Pipe and Redirection
    "cat < file",

    // Test 2: Pipe and Redirection
    "< file",

    // Test 3: Strings with Double and Single Quotes
    "echo \"hello world\" 'foo bar'",

    // Test 4: Operators Inside Quotes
    "echo \"a|b\" | grep \"c>d\"",

    // Test 5: Consecutive redirections (heredoc + append)
    "cat << heredoc >> out",

    // Test 6: Only quoted spaceg_exit_status
    "' '",

    // Test 7: Double quotes with embedded dollar (allowed in double quotes)
    "echo \"The price is $100\"",

    // Test 8: Single quotes with dollar (literal)
    "echo 'The price is $100'",

    // Test 9: Mixed quote styles
    "echo \"it's a test\"",

    // Test 10: Heredoc with quoted delimiter
    "cat << 'EOF'",

    // Test 11: Multiple pipes
    "cmd1 | cmd2 | cmd3 | cmd4",

    // Test 12: Many spaces between arguments
    "echo       hello      world",

    // Test 13: Quoted pipe + unquoted pipe
    "echo 'this | is not pipe' | wc -l",

    // Test 14: Redirection with no filename (still valid to tokenize)
    "cat >",

    // Test 15: Redirection then commandint main(int argc, char **argv, char **envp)
    "> out.txt echo hello",

    "echo hello",
    
    // ✅ Valid test cases but on the edge

    // Test  continuous redirections
    "cat << infile1 << infile2 << infile3 >> out4",

    //not cmd
    "< file",
    
    //no file
    "cat <<",

    //expansion
    "echo $USER",
    
    //empty string
    "",
//==========================================================================
    // ❌ Syntax error test cases (unclosed quotes, invalid input)

    // ❌ Test 18: Unclosed double quote
    "echo \"hello world",

    // ❌ Test 19: Unclosed single quote
    "echo 'hello world",

    // ❌ Test 20: Mixed quote unclosed
    "echo 'hello \"world",

    // ❌ Test 21: Complex nesting with unclosed
    "echo 'this is \"broken",

    // ❌ Test 22: Only quote
    "'",

    // ❌ Test 23: Backslash (not required by subject)
    "echo hello\\world",
  
    // ❌ Test 24: Semicolon (not required)
    "echo hello; echo world",

    // ❌ Test 25: Mix of unsupported and supported
    "cat file.txt \\; echo done",

    // ❌ Test 26: Escaped quotes (not required)
    "echo \\\"hello\\\"",
//==========================================================================
    // ❌ after lexing
    // Test 27: Pipe at beginning
    "| cat file.txt",

    //Test 28: Pipe at end (missing RHS)
    "cat file.txt |",

    // ❌  Test 29: Multiple pipes in a row
    "cat file.txt || grep hello",

    // ❌  Test 30: Redirection without target
    "cat <",

    // ❌  Test 31: Multiple redirections without file
    "cat <<",

    // ❌  Test 32: Redirection to pipe
    "cat > | grep hi",

    // ❌  Test 33: Consecutive redirection and pipe
    "cat <| grep",

    // ❌  Test 34: Output redirection with no command
    "> file",

    // ❌  Test 35: Pipe followed by redirection
    "| > file",

    // ❌  Test 36: Invalid token between redirection
    "cat > | file",

    // ❌  Test 37: Empty input
    "",

    // ❌   Test 38: Only pipe
    "|",

    // ❌   Test 39: Multiple redirection symbols in argument
    "cat >>> file.txt",

    // ❌   Test 40: Redirection with special character
    "cat > ; file.txt",

    // ❌   Test 41: Pipe inside quotes (should not be parsed as pipe)
    "echo 'this | is literal'",
    //    ✅ This one is valid if quotes are handled properly, but good to test that the lexer/parser does not misinterpret it.

    // ❌   Test 42: Incomplete heredoc
    "cat << EOF",

        // Test 16: Spaces only
    "        ",

    // Test 17: Empty string
    "",

    NULL

};

// static void show_real_bash_tokens(const char *input)
// {
//     char command[4096];
//     char sanitized[2048];

//     sanitize_input(input, sanitized, sizeof(sanitized));

//     snprintf(command, sizeof(command),
//         "bash -c 'set -- %s; i=1; for arg in \"$@\"; do echo \"$i: [$arg]\"; i=$((i+1)); done'",
//         sanitized);

//     FILE *fp = popen(command, "r");
//     if (!fp)
//     {
//         perror("popen");
//         return;
//     }

//     printf("🐚 Real Bash Tokens:\n");
//     char line[512];
//     while (fgets(line, sizeof(line), fp))
//         printf("%s", line);

//     pclose(fp);
// }	

static void print_token_list(t_token *head)
{
    int i = 0;
    if (!head)
    {
        printf("Tokens list is empty\n");
        return;
    }
    while (head)
    {
        printf("Token[%d]: ", i++);
        // Print token type as string inline
        if (head->t_type == T_WORD)             printf("type=T_WORD (0), ");
        else if (head->t_type == T_PIPE)       printf("type=T_PIPE (1), ");
        else if (head->t_type == T_REDIRECT_IN)printf("type=T_REDIRECT_IN (2), ");
        else if (head->t_type == T_REDIRECT_OUT)printf("type=T_REDIRECT_OUT (3), ");
        else if (head->t_type == T_APPEND)     printf("type=T_APPEND (4), ");
        else if (head->t_type == T_HEREDOC)    printf("type=T_HEREDOC (5), ");
        else                                  printf("type=UNKNOWN (%d), ", head->t_type);

        // Print quote type inline
        if (head->quote_type == 0)              printf("quote=NONE (0), ");
        else if (head->quote_type == 1)         printf("quote=SINGLE_QUOTE (1), ");
        else if (head->quote_type == 2)         printf("quote=DOUBLE_QUOTE (2), ");
        else                                   printf("quote=UNKNOWN (%d), ", head->quote_type);

        printf("str='%s'\n", head->str ? head->str : "(null)");

        head = head->next;
    }
}

static void print_cmd_list(t_cmd *head)
{
    int cmd_i = 0;
    if (!head)
    {
        printf("Commands list is empty\n");
        return;
    }

    while (head)
    {
        printf("Command[%d]:\n", cmd_i++);

        // Print argv array and corresponding quote_type array
        if (!head->argv)
        {
            printf("  argv: (null)\n");
        }
        else
        {
            printf("  argv:\n");
            for (int i = 0; head->argv[i] != NULL; i++)
            {
                printf("    argv[%d]: '%s'", i, head->argv[i]);
                if (head->quote_type)
                {
                    int q = head->quote_type[i];
                    if (q == 0)      printf(" (quote=NONE)");
                    else if (q == 1) printf(" (quote=SINGLE_QUOTE)");
                    else if (q == 2) printf(" (quote=DOUBLE_QUOTE)");
                    else             printf(" (quote=UNKNOWN %d)", q);
                }
                printf("\n");
            }
        }

        // Print redirections
        if (!head->redirections)
        {
            printf("  redirections: (none)\n");
        }
        else
        {
            printf("  redirections:\n");
            t_redir *r = head->redirections;
            int r_i = 0;
            while (r)
            {
                printf("    redir[%d]: ", r_i++);
                if (r->type == T_REDIRECT_IN)       printf("type=T_REDIRECT_IN (2), ");
                else if (r->type == T_REDIRECT_OUT) printf("type=T_REDIRECT_OUT (3), ");
                else if (r->type == T_APPEND)        printf("type=T_APPEND (4), ");
                else if (r->type == T_HEREDOC)       printf("type=T_HEREDOC (5), ");
                else                                printf("type=UNKNOWN (%d), ", r->type);

                printf("file='%s'", r->file ? r->file : "(null)");
                if (r->type == T_HEREDOC)
                    printf(", heredoc_delim='%s'", r->heredoc_delim ? r->heredoc_delim : "(null)");

                printf("\n");
                r = r->next;
            }
        }

        head = head->next;
    }
}


int main(int argc, char **argv, char **envp)
{
    t_token *tokens;
    t_cmd   *cmds;
    int     i = 0;
	t_env		*env_list;
	t_exec_path	exec_cmd;

	(void)argc;
	(void)argv;
     
	ft_memset(&exec_cmd, 0, sizeof(exec_cmd));
	env_list = env_list_init(envp);
	if (!env_list)
	{
		ft_putstr_fd("Error: env list initialized failed\n", 2);
		exit(EXIT_FAILURE);
	}
	exec_cmd.envp = env_list_to_envp(env_list);
	if (!exec_cmd.envp)
	{
		free_env_list(env_list);
		ft_putstr_fd("Error: env list initialized failed\n", 2);
		exit(EXIT_FAILURE);
	}
    while (test_lines[i])
    {
        tokens = NULL;
        cmds = NULL;
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("Test %d: %s\n", i + 1, test_lines[i]);
        tokens = get_token_list(test_lines[i]);
        if (!tokens)
              printf("KOOOOOOOOO build tokens failure\n");
        else
        {
            printf("🧱 Tokens:\n");
            print_token_list(tokens);
        } 
        // show_real_bash_tokens(test_lines[i]);
        printf("━━━━━━━━━━━━━━━━\n");
        cmds = build_command_list(tokens);
        if(tokens)
            free_token_list(tokens);
        if (cmds)
        {
            expand_all_cmds(cmds, exec_cmd.envp);
            printf("\n🔧 Commands:\n");
            print_cmd_list(cmds);
        }
        else
            printf("KOOOOOOOOO build cmd failure\n");
        if (cmds)
            free_cmd_list(cmds);
        i++;
        g_exit_status = 0;
    }
    free_t_exec_path(&exec_cmd);
	free_env_list(env_list);
    return (0);
}

