#include "minishell.h"

int	g_exit_status = 0;

// static void print_token_list(t_token *head)
// {
//     if (!head)
//     {
//         printf("token empty\n");
//         return;
//     }
//     while (head)
//     {
//         printf("[TYPE:%d] ", head->t_type);
//         if (head->quote_type == 1)
//             printf("(SINGLE-QUOTE) ");
//         else if (head->quote_type == 2)
//             printf("(DOUBLE-QUOTE) ");
//         printf("VAL:'%s'; ", head->str ? head->str : "(null)");
//         head = head->next;
//     }
//     printf("\n");
// }

// static void print_cmd_list(t_cmd *head)
// {
//     int i = 0;
//     int cmd_num = 1;

//     if (!head)
//     {
//         printf("cmd empty\n");
//         return ;
//     }
//     while (head)
//     {
//         printf("====== Command %d ======\n", cmd_num++);
//         // Print argv
//         if (head->argv)
//         {
//             printf("argv: ");
//             i = 0;
//             while (head->argv[i])
//             {
//                 printf("'%s' ", head->argv[i]);
//                 i++;
//             }
//             printf("\n");
//         }
//         else
//             printf("argv: (null)\n");
//         // Print input redicheck_quotes_closedrection
//         if (head->infile)
//             printf("infile: '%s'\n", head->infile);
//         // Print output redirection
//         if (head->outfile)
//         {
//             printf("outfile: '%s'\n", head->outfile);
//             printf("append_out: %s\n", head->append_out ? "yes" : "no");
//         }
//         // Print heredoc delimiter
//         if (head->heredoc_delim)
//             printf("heredoc_delim: '%s'\n", head->heredoc_delim);
//         head = head->next;
//     }
// }


int main(int argc, char **argv, char **envp)
{
	t_env		*env_list;
	char		*line;
	t_token		*token_list;
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
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if(*line)
		{
			add_history(line);
			//convert list to envp array
			exec_cmd.envp = env_list_to_envp(env_list);
			if (!exec_cmd.envp)
			{
				free(line);
				free_env_list(env_list);
				ft_putstr_fd("Error: env list initialized failed\n", 2);
				exit(EXIT_FAILURE);
			}
			//convert line token list
			token_list = get_token_list(line);
			free(line);
			if (!token_list)
			{
				free_env_list(env_list);
				ft_free_arr(exec_cmd.envp);
				ft_putstr_fd("Error: get token list failed\n", 2);
				exit(EXIT_FAILURE);
			}
            //print tokens;
            //print_token_list(token_list);
			
            //expand_all_tokens(token_list, exec_cmd);

			//convert token list to command list
			exec_cmd.whole_cmd = build_command_list(token_list);
			free_token_list(token_list);

            //print cmd
            //print_cmd_list(exec_cmd.whole_cmd);
			if (!exec_cmd.whole_cmd)
			{
				free_env_list(env_list);
				ft_free_arr(exec_cmd.envp);
				ft_putstr_fd("Error: build command list failed\n", 2);
                free_t_exec_path(&exec_cmd);
				exit(EXIT_FAILURE);
			}

            expand_all_cmds(exec_cmd.whole_cmd, exec_cmd.envp);
			//check < infile
			if (!exec_cmd.whole_cmd->argv || !exec_cmd.whole_cmd->argv[0])
			{
				if (check_and_apply_redirections(exec_cmd.whole_cmd) == -1)
					g_exit_status = 1;
				else
					g_exit_status = 0;
                free_t_exec_path(&exec_cmd);
				continue;
			}
			//check if null or empty
			if (exec_cmd.whole_cmd->argv[0][0] == '\0')
			{
				ft_putstr_fd("minishell: : command not found\n", 2);
				g_exit_status = 127;
                free_t_exec_path(&exec_cmd);
				continue;
			}
			//if bulitin, no need to find cmd_path, just execute(need to deal with other things in it)
			if (is_builtin(exec_cmd.whole_cmd->argv[0]))
			{
				exec_cmd.cmd_path = NULL;
				run_builtin_with_redir(exec_cmd.whole_cmd, &env_list);
                free_t_exec_path(&exec_cmd);
				continue;
			}
			//if internal cmd, get cmd_path first, then run external cmd
			else
			{
				exec_cmd.cmd_path = get_cmd_path(exec_cmd.whole_cmd->argv[0], exec_cmd.envp);
				if (!exec_cmd.cmd_path)
				{
					ft_putstr_fd(exec_cmd.whole_cmd->argv[0], 2);
					ft_putstr_fd(": command not found\n", 2);
                    free_t_exec_path(&exec_cmd);
					g_exit_status = 127;
					continue;
				}
				execute_external_cmd(&exec_cmd);
                free_t_exec_path(&exec_cmd);
				continue;
			}
		}
		//free_t_exec_path(&exec_cmd);
		rl_clear_history();
	}
	free_env_list(env_list);
	return (0);
}

#include <setjmp.h> //delete later
jmp_buf g_jmpbuf;//delete later

/* 
char *test_lines[] =
{
    // ✅ Valid test cases

    // Test 1: Simple Command
    "echo hello world",

    // Test 2: Pipe and Redirection
    "cat < file | grep foo > out.txt",

    // Test 3: Strings with Double and Single Quotes
    "echo \"hello world\" 'foo bar'",

    // Test 4: Operators Inside Quotes
    "echo \"a|b\" | grep \"c>d\"",

    // Test 5: Consecutive redirections (heredoc + append)
    "cat << heredoc >> out",

mikko@mikko-desktop-ubuntu:~/yuxin_home/minishell$ echo << $USER
> skfd
> mikko
> $USER

mikko@mikko-desktop-ubuntu:~/yuxin_home/minishell$ cat $USER
cat: mikko: No such file or directory
mikko@mikko-desktop-ubuntu:~/yuxin_home/minishell$ echo $USER
mikko
mikko@mikko-desktop-ubuntu:~/yuxin_home/minishell$ ./minishell 
minishell$  echo << $USER
minishell: heredoc> asd
asd
minishell: heredoc> mikko


    // Test 6: Only quoted space
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

    // Test 15: Redirection then command
    "> out.txt echo hello",

    // Test 16: Spaces only
    "        ",

    // Test 17: Empty string
    "",
//==========================================================================
    // ❌ Syntax error test cases (unclosed quotes, invalid input)

    // ❌ Test 18: Unclosed double quote
    "echo \"hello world",
./m
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
    "> file.txt",

    // ❌  Test 35: Pipe followed by redirection
    "| > file.txt",

    // ❌  Test 36: Invalid token between redirection
    "cat > | file.txt",

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

    // ✅   Test 42: Incomplete heredoc
    "echo hello << test",

    NULL
};

//about quotes and #USER
echo "yuxin $USER '$USER' "user" kk,"
echo "hello "yuxin" wi"
echo 'hello 'yuxin' wi'
echo "hello 'yuxin' wi"
echo 'hello "yuxin" wi'

//about redirection
< infile            bash: infile: No such file or directory, fake excution
cat <               bash: syntax error near unexpected token 'newline', exit during parsing stage

//cmds without empty space in between
echo$USER           bash: echoyuwu: command not found
cat<Makefile        bash: solid, same as cat < Makefile

//for testing lexing, delete later
static void print_token_list(t_token *head)
{
    if (!head)
    {
        printf("token empty\n");
        return;
    }
    while (head)
    {
        printf("[TYPE:%d] ", head->t_type);
        if (head->quote_type == 1)
            printf("(SINGLE-QUOTE) ");
        else if (head->quote_type == 2)
            printf("(DOUBLE-QUOTE) ");
        printf("VAL:'%s'; ", head->str ? head->str : "(null)");
        head = head->next;
    }
    printf("\n");
}

// For testing parsing, delete later
static void print_cmd_list(t_cmd *head)
{
    int i = 0;
    int cmd_num = 1;

    if (!head)
    {
        printf("cmd empty\n");
        return ;
    }
    while (head)
    {
        printf("====== Command %d ======\n", cmd_num++);
        // Print argv
        if (head->argv)
        {
            printf("argv: ");
            i = 0;
            while (head->argv[i])
            {
                printf("'%s' ", head->argv[i]);
                i++;
            }
            printf("\n");
        }
        else
            printf("argv: (null)\n");
        // Print input redicheck_quotes_closedrection
        if (head->infile)
            printf("infile: '%s'\n", head->infile);
        // Print output redirection
        if (head->outfile)
        {
            printf("outfile: '%s'\n", head->outfile);
            printf("append_out: %s\n", head->append_out ? "yes" : "no");
        }
        // Print heredoc delimiter
        if (head->heredoc_delim)
            printf("heredoc_delim: '%s'\n", head->heredoc_delim);
        head = head->next;
    }
}

#include <string.h>

// Helper to sanitize input by escaping redirection chars
static void sanitize_input(const char *input, char *out, size_t out_size)
{
    size_t j = 0;
    for (size_t i = 0; input[i] && j + 2 < out_size; i++)
    {
        if (input[i] == '<' || input[i] == '>' || input[i] == '|')
        {
            // Escape with backslash to prevent redirection execution
            out[j++] = '\\';
            out[j++] = input[i];
        }
        else
        {
            out[j++] = input[i];
        }
    }
    out[j] = '\0';
}

static void show_real_bash_tokens(const char *input)
{
    char command[4096];
    char sanitized[2048];

    sanitize_input(input, sanitized, sizeof(sanitized));

    snprintf(command, sizeof(command),
        "bash -c 'set -- %s; i=1; for arg in \"$@\"; do echo \"$i: [$arg]\"; i=$((i+1)); done'",
        sanitized);

    FILE *fp = popen(command, "r");
    if (!fp)
    {
        perror("popen");
        return;
    }

    printf("🐚 Real Bash Tokens:\n");
    char line[512];
    while (fgets(line, sizeof(line), fp))
        printf("%s", line);

    pclose(fp);
}



int main(void)
{
    t_token *tokens;
    t_cmd   *cmds;
    int     i = 0;

    while (test_lines[i])
    {
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("Test %d: %s\n", i + 1, test_lines[i]);
        if (setjmp(g_jmpbuf) != 0)
        {
            printf("cmd empty\n");
            i++;
            continue;
        }
        tokens = get_token_list(test_lines[i]);
        printf("🧱 Tokens:\n");
        if (!tokens)
              printf("tokens empty\n");
        else
            print_token_list(tokens);
        show_real_bash_tokens(test_lines[i]);
        printf("━━━━━━━━━━━━━━━━\n");
        cmds = build_command_list(tokens);
        if (!cmds)
            printf("cmd empty\n");
        else
        {
            printf("\n🔧 Commands:\n");
            print_cmd_list(cmds);
            free_token_list(tokens);
            free_cmd_list(cmds);
        }
        i++;
    }
    return (0);
}
*/
