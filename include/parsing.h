#ifndef PARSING_H
# define PARSING_H

/*
readline            - get a line from a user with editing       get a line from a user with editing
*/

#include <readline/history.h>       //add_history
#include <readline/readline.h>      //readline
# include <stdio.h>                 //readline
# include <stdlib.h>                //malloc, free

//tokeniser: get a raw line and change it to a linked list of minimal unit(tokens)
//eg: echo "hello world" > file.txt | cat -e
//  [TOKEN: echo]   [TOKEN: "hello world"]  [TOKEN: >]  [TOKEN: file.txt]   [TOKEN: |]  [TOKEN: cat]    [TOKEN: -e]
//     word(word)            word           direct out   word(filename)        pipe      word(cmd)          word
//theses little pieces are of the types below:

/* 

typedef enum e_token_type 
{
	T_WORD,             // 0    // "string" :   string, a cmd, a arguement
	T_PIPE,             // 1    // |        :   pipe
	T_REDIRECT_IN,      // 2    // <        :   input redirection
	T_REDIRECT_OUT,     // 3    // >        :   output redrection
	T_APPEND,           // 4    // >>       :   output appen
	T_HEREDOC,          // 5    // <<       :   heredoc input
}	t_token_type;

//Do I used ft_split and then make a linked list with the str of strings?
typedef struct      s_token
{
    char            *str;
    t_token_typo    t_type;
    struct s_token  *next;
}                   t_token;

// lex.c
// get raw_line with readline / getnextline, ( then use ft_split), and the put them to t_teken
t_token    *get_tokens(char *raw_line);
void        fine_single_quotes(char *raw_line);
void        find_double_quetes(char *raw_line);
int         get_token_nbr(char  *raw_line);

// parser.c       
// syntax check (pipes, etc.)                  
int check_syntax(t_token *tokens);
// build command structures                    
t_cmd *build_command_list(t_token *tokens);
// handle redirections                        
void parse_redirections(t_cmd *cmd, t_token **tokens);
// group commands by pipe                      
t_cmd *group_by_pipes(t_token *tokens);                   

*/               

#endif
