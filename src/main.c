#include "minishell.h"
# include <stdio.h>

char *test_lines[] = {
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

    // Test 17: Spaces only
    "        ",

    // Test 18: Empty string
    "",
//==========================================================================
    // ❌ Syntax error test cases (unclosed quotes, invalid input)

    // ❌ Test 1: Unclosed double quote
    "echo \"hello world",

    // ❌ Test 2: Unclosed single quote
    "echo 'hello world",

    // ❌ Test 3: Unclosed single quote
    "echo 'hello world",

    // ❌ Test 4: Unclosed double quote
    "echo \"hello world",

    // ❌ Test 5: Mixed quote unclosed
    "echo 'hello \"world",

    // ❌ Test 6: Quote opens then escapes
    "echo \"hello",

    // ❌ Test 7: Complex nesting with unclosed
    "echo 'this is \"broken",

    // ❌ Test 7: Only quote
    "'",

    // ❌ Test 8: Backslash (not required by subject)
    "echo hello\\world",

    // ❌ Test 9: Semicolon (not required)
    "echo hello; echo world",

    // ❌ Test 10: Mix of unsupported and supported
    "cat file.txt \\; echo done",

    // ❌ Test 11: Escaped quotes (not required)
    "echo \\\"hello\\\"",

    // Test 16: Pipe at beginning
    "| cat file.txt",

    NULL
};

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
        printf("cmd empty\n");
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

static void show_real_bash(const char *input)
{
    char command[2048];
    int status;

    // Prepare bash command to check syntax only (-n)
    snprintf(command, sizeof(command),
        "bash -n -c '%s' 2>/dev/null", input);

    status = system(command);

    if (WIFEXITED(status))
    {
        int code = WEXITSTATUS(status);
        if (code == 0)
            printf("Syntax OK (Bash Exit Status: %d)\n", code);
        else
            printf("Syntax Error (Bash Exit Status: %d)\n", code);
    }
    else
    {
        printf("🐚 Bash did not exit normally\n");
    }
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
        tokens = get_token_list(test_lines[i]);
        printf("🧱 Tokens:\n");
        if (!tokens)
              printf("tokens empty\n");
        else
            print_token_list(tokens);
        show_real_bash(test_lines[i]);
        printf("━━━━━━━━━━━━━━━━\n");
        cmds = build_command_list(tokens);
        printf("\n🔧 Commands:\n");
        if (!cmds)
            printf("cmd empty\n");
        else
            print_cmd_list(cmds);
        free_token_list(tokens);
        free_cmd_list(cmds);
        i++;
    }
    return (0);
}
