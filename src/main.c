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

int main(void)
{
    char    line[50] = "cat < file | grep foo > out.txt";
    t_token *result;

    result = get_token_list(line);
    printf("so far so good\n");
    print_token_list(result);
    free_token_list(result);
}