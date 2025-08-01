#ifndef PARSING_H
# define PARSING_H

# include <unistd.h>         //access, access, close, fork
# include <stdlib.h>         //malloc, free
# include <fcntl.h>
# include <stdio.h>
# include "exec.h"

//Lexing: get a raw line and change it to a linked list of minimal unit(tokens)
typedef enum e_token_type
{
	T_WORD,             // 0    // "string" :   string, a cmd, a arguement
	T_PIPE,             // 1    // |        :   pipe
	T_REDIRECT_IN,      // 2    // <        :   input redirection   :   Reads from a file
	T_REDIRECT_OUT,     // 3    // >        :   output redrection   :   Writes to a file, replacing its contents.
	T_APPEND,           // 4    // >>       :   output appen        :   Writes to a file, but appends at the end.
	T_HEREDOC,          // 5    // <<       :   heredoc input       :   Feeds inline text as stdin.
}	t_token_type;

typedef struct      s_token
{
    char            *str;
    t_token_type    t_type;
    int				quote_type;		//0: no quote; 1: single quote; 2: double quote
	struct s_token  *next;
}                   t_token;

//Parsing: - Taking the token list and Understanding the structure of the command,
// 			- Grouping tokens into command nodes, pipes, redirections, etc,
// 			- Building a linked list of command objects (t_cmd)
typedef struct s_redir 
{
	char			*file;
	int				type; // T_REDIRECT_IN, T_REDIRECT_OUT, T_APPEND, T_HEREDOC
    char            *heredoc_delim;
	struct s_redir	*next;
}					t_redir;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirections;  // <-- list of redirections
	struct s_cmd    *next;
} t_cmd;

/*   old one
typedef struct s_cmd
{
    char            **argv;         // Arguments array: argv[0] = command, argv[1..n] = args
    char            *infile;        // For '<' redirection ::           cat < in < out  *infile
    char            *outfile;       // For '>' or '>>' redirection      cat > in > out **outfile
    int             append_out;     // 1 if '>>', 0 if '>'
    char            *heredoc_delim; // For '<<' heredoc
    struct s_cmd    *next;          // Next command in a pipeline
}                   t_cmd;
*/

// lex_utils.c
int		check_raw_line_syntax(char *raw_line);
void	free_token_list(t_token *token_head);
void	get_token_type(t_token *token);
void	get_quote_type(t_token *token, char q);

// lex.c
// get raw_line with readline / getnextline, and the put them to t_token
t_token	*get_token_list(char *raw_line);
t_token	*build_word_token(char *line, int *i);
t_token	*build_token_from_next_word(char *line, int *i);
char	*get_quoted_part(char *s, int *i);
char	*get_unquoted_part(char *s, int *i);

// parser_utils.c
int         check_special_characters(t_token *token_head);
int				check_token_syntax(t_token *tokens);
int				count_argv(t_token *start);
void		free_redirections(t_cmd *cmd_head);
void		free_cmd_list(t_cmd *cmd_head);

// parser.c
//change token_list to command list and free the original tokens
t_cmd		*build_command_list(t_token *tokens);
t_token		*get_one_new_cmd(t_token    *token_head, t_cmd *cmd_current);
t_token		*parse_redirections(t_cmd *cmd, t_token *tokens);
t_token		*parse_argv(t_cmd *cmd, t_token *tokens);

//expander_utils.c >
//string operations helpers
// char	*get_env_value(char **envp, const char *key);
// char	*replace_variable_in_str(char *input, int pos, char **envp);

//expander.c >...... this one should be called after lexing,
//check the t_word without quotes or with double quotes, when it starts with $ and if it exits
//not expanding heredocs
char	*expand_variables_in_str(char *input, char **envp);
void	expand_redirection(t_cmd *cmd_list, char **envp);
void	expand_all_cmds(t_cmd *cmd_list, char **envp);

#endif
