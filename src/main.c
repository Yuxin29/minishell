#include "minishell.h"
# include <stdio.h>

//for testing, delete later
static void print_token_list(t_token *head)
{
    while (head)
    {
        printf("[TYPE:%d] ", head->t_type);
        if (head->quote_type == 1)
            printf("(SINGLE-QUOTE) ");
        else if (head->quote_type == 2)
            printf("(DOUBLE-QUOTE) ");
        printf("VAL:'%s'\n", head->str ? head->str : "(null)");
        head = head->next;
    }
}

// For testing, delete later
static void print_cmd_list(t_cmd *head)
{
    int i = 0;
    int cmd_num = 1;

    while (head)
    {
        printf("=== Command %d ===\n", cmd_num++);

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

        // Print input redirection
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

int main(void)
{
    //char    line[] = "cat < input.txt | grep foo > out.txt | echo \"hello world\" 'again' >> out2.txt";
    //char    line[] = "echo hello world";
    char    line[] = "cat << heredoc >> out";
    t_token *tokens;
    t_cmd   *cmds;

    tokens = get_token_list(line);
    printf("🧱 Tokens:\n");
    print_token_list(tokens);          // Your existing token debug
    cmds = build_command_list(tokens);
    printf("\n🔧 Commands:\n");
    print_cmd_list(cmds);              // New printer
    free_token_list(tokens);
    free_cmd_list(cmds);               // You’ll need to write this helper to free everything
}

/*
======================================testing script=============================================
// Test 1: Simple Command
Input: echo hello world
Expected:
[TOKEN_WORD:echo][TOKEN_WORD:hello][TOKEN_WORD:world]

// Test 2: Pipe and Redirection
Input: cat < file | grep foo > out.txt
Expected:
[TOKEN_WORD:cat][TOKEN_REDIR_IN:<][TOKEN_WORD:file][TOKEN_PIPE:|]
[TOKEN_WORD:grep][TOKEN_WORD:foo][TOKEN_REDIR_OUT:>][TOKEN_WORD:out.txt]

// Test 3: Strings Quoted with Single/Double Quotes
Input: echo "hello world" 'foo bar'
Expected:[TOKEN_WORD:echo][TOKEN_WORD:hello world:quoted=2][TOKEN_WORD:foo bar:quoted=1]

// Test 4: Operators Inside Quotes
Input: echo "a|b" | grep "c>d"
Expected:
[TOKEN_WORD:echo][TOKEN_WORD:a|b:quoted=2][TOKEN_PIPE:|]
[TOKEN_WORD:grep][TOKEN_WORD:c>d:quoted=2]

// Test 5: Escaped Characters
Input: echo \"hello\"
Expected:
[TOKEN_WORD:echo][TOKEN_WORD:"hello"]

// Test 6: Multiple Consecutive Operators
Input: cat << heredoc >> out
Expected:
[TOKEN_WORD:cat][TOKEN_HEREDOC:<<][TOKEN_WORD:heredoc]
[TOKEN_REDIR_APPEND:>>][TOKEN_WORD:out]

// Test 7: Only Spaces
Input: " "
Expected:
[TOKEN_WORD: :quoted=2]

// Test 8: Empty String
Input: (empty)
Expected:
(NULL or empty list)

// Test 9: Unclosed Quotes
Input: echo "hello world
Expected: Error or TOKEN_UNKNOWN (lexer should detect the error)

// Test 10: Token Containing Special Characters
Input: echo ab\$c
Expected:[TOKEN_WORD:echo][TOKEN_WORD:ab$c]
(The lexer should correctly interpret the escaped character)
*/