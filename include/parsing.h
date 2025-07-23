#ifndef PARSING_H
# define PARSING_H

/*
readline            - get a line from a user with editing       get a line from a user with editing
*/

#include <readline/history.h>       //add_history
#include <readline/readline.h>      //readline
# include <stdio.h>                 //readline
# include <stdlib.h>                //malloc, free

#define SINGLE_QUOTE '''
#define DOUBLE_QUOTE '"'

typedef enum e_token_type 
{
	T_WORD,             // 0    // "string" :   string, a cmd, a arguement
	T_PIPE,             // 1    // |        :   pipe
	T_REDIRECT_IN,      // 2    // <        :   input redirection
	T_REDIRECT_OUT,     // 3    // >        :   output redrection
	T_APPEND,           // 4    // >>       :   output appen
	T_HEREDOC,          // 5    // <<       :   heredoc input
}	t_token_type;

typedef struct      s_token
{
    char            *str;
    t_token_type    t_type;
    int				quote_type;		//0: no quote; 1: single quote; 2: double quote
	struct s_token  *next;
}                   t_token;
//Lexing: get a raw line and change it to a linked list of minimal unit(tokens)

typedef struct s_cmd
{
    char            **argv;         // Arguments array: argv[0] = command, argv[1..n] = args
    char            *infile;        // For '<' redirection
    char            *outfile;       // For '>' or '>>' redirection
    int             append_out;     // 1 if '>>', 0 if '>'
    char            *heredoc_delim; // For '<<' heredoc
    struct s_cmd    *next;          // Next command in a pipeline
}                   t_cmd;
//Parsing: - Taking the token list, 
// 			-Understanding the structure of the command, 
// 			- Grouping tokens into command nodes, pipes, redirections, etc,
// 			- Building a tree or linked list of command objects (t_cmd)

//for example
//echo "hello world" > file.txt | cat -e
//[TOKEN: echo]	[TOKEN: "hello world"]	[TOKEN: >]	[TOKEN: file.txt]   [TOKEN: |] 	[TOKEN: cat]  	[TOKEN: -e] 
//T_WORD	  				T_WORD		T_REDIRECT_OUT		 T_WORD			  T_PIPE	   T_WORD		  T_WORD

//t_cmd *cmd_list = cmd1;
//cmd1:
//  argv:        ["echo", "hello world"]
//  outfile:     "file.txt"
//  next:        cmd2 →
//cmd2:
//  argv:        ["cat", "-e"]
//  next:        NULL

// lex.c
// get raw_line with readline / getnextline, ( then use ft_split), and the put them to t_teken
t_token    *get_tokens(char *raw_line);
t_token    *get_one_new_token(char *raw_line);
t_token    *add_one_more_token(t_token *current, char *raw_line);

// lex_utils.c
void	get_token_type(t_token *token);
void    free_token_list(t_token *token);

// parser.c       
// syntax check (pipes, etc.)                  
int		check_syntax(t_token *tokens);
// build command structures                    
t_cmd	*build_command_list(t_token *tokens);
// handle redirections                        
void	parse_redirections(t_cmd *cmd, t_token **tokens);
// group commands by pipe                      
t_cmd	*group_by_pipes(t_token *tokens);

// parser_utils.c
           

#endif
