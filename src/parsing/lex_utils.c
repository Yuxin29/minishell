#include "minishell.h"

void	get_token_type(t_token *token)
{
    const char *tmp;

    tmp = token->str;
    if (ft_strncmp(tmp, "|", 2) == 0)
        token->t_type = T_PIPE;
    else if (ft_strncmp(tmp, "<", 2) == 0)
        token->t_type = T_REDIRECT_IN;
    else if (ft_strncmp(tmp, ">", 2) == 0)
        token->t_type = T_REDIRECT_OUT;
    else if (ft_strncmp(tmp, "<<", 3) == 0)
        token->t_type = T_APPEND;
    else if (ft_strncmp(tmp, ">>", 3) == 0)
        token->t_type = T_HEREDOC;
    else
        token->t_type = T_WORD;
}

//free the whole linked list
void    free_token_list(t_token *head)
{
    t_token *tmp;

    while(head)
    {
        tmp = head->next;
        if (head->str)
            free(head->str);
        free(head);
        head = tmp;
    }
}

/*
cc -Wall -Wextra -Werror -I./include -I./libft ./src/parsing/lex.c ./src/parsing/lex_utils.c
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