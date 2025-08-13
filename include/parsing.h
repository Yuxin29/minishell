#ifndef PARSING_H
# define PARSING_H

// syntax error ms macors
#define SYNTAX_ERR_PIPE					"minishell: syntax error near unexpected token `|'"
#define SYNTAX_ERR_REDIR_DOUBLE			"minishell: syntax error: near unexpected token redirections"
#define SYNTAX_ERR_REDIR_FILE_MISSING	"minishell: syntax error: missing filename or delimiter after redirection"
#define SYNTAX_ERR_SPECIAL_CHARS		"minishell: syntax error: special characters"
#define SYNTAX_ERR_QUOTES				"minishell: syntax error: unclosed quotes"

//common shell cmd line length
#define BUF_SIZE 8192

//needed for expand, bit used yet
#define EMPTY ""

//Needed from exec.h
typedef struct s_exec_path t_exec_path;

// 	T_WORD,			0    "string" 	string				a cmd, a arguement
// 	T_PIPE,			1    |      	pipe
// 	T_REDIRECT_IN,	2     <      	input redirection   Reads from a file
// 	T_REDIRECT_OUT,	3     >       	output redrection 	Writes to a file, replacing its contents.
// 	T_APPEND,       4     >>    	output appen       	writes to a file, but appends at the end.
// 	T_HEREDOC,      5    <<   		heredoc input      	Feeds inline text as stdin.
typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIRECT_IN,
	T_REDIRECT_OUT,
	T_APPEND,
	T_HEREDOC,
}	t_token_type;

//quote_type;		//0: no quote; 1: single quote; 2: double quote, 3: QUOTE_DOLLAR_DOUBLE
typedef struct		s_token
{
	char			*str;
	t_token_type	t_type;
	int				quote_type;
	struct s_token	*next;
}	t_token;

 // list of redirections
// int	type; inherete from t_token: 2 - 5
typedef struct s_redir
{
	char			*file;
	int				type;
	char			*heredoc_delim;
	struct s_redir	*next;
}	t_redir;

//quote_type; inherete from t_token: 0; 1; 2
typedef struct s_cmd
{
	char			**argv;
	int				*quote_type;
	t_redir			*redirections;
	char			*cmd_path;
	struct s_cmd	*next;
}	t_cmd;

//pre_expander.c
char	*pre_expand_line(t_exec_path *cmd, char *raw_line, char **envp);

// lex_utils.c
void	check_raw_line_syntax(char *raw_line, t_exec_path *cmd);
void	get_token_type(t_token *token);
void	get_quote_type(t_token *token, char q);
char	*get_unquoted_part(char *s, int *i);
char	*get_quoted_part(char *s, int *i);

// lex.c
// get a raw line and change it to a linked list of minimal unit(tokens)
t_token	*get_token_list(t_exec_path *cmd, char *raw_line);
t_token	*build_word_token(char *line, int *i);
t_token *build_operator_token(char *line, int *i);
t_token	*build_token_from_next_word(char *line, int *i);

// parser_utils.c
int		check_special_characters(t_token *token_head);
void	check_token_syntax(t_token *token_head, t_exec_path *cmd);
int		count_argv(t_token *start);

// parser.c
// change token_list to command list and free the original tokens
// - Taking the token list and Understanding the structure of the command,
// - Grouping tokens into command nodes, pipes, redirections, etc,
// - Building a linked list of command objects (t_cmd)
t_cmd	*build_command_list(t_exec_path *cmd, t_token *tokens);
t_token	*get_one_new_cmd(t_token    *token_head, t_cmd *cmd_current);
t_redir	*create_redir_node(t_token *redir_tok, t_token *file_tok);
t_token	*parse_redirections(t_cmd *cmd, t_token *tokens);
t_token	*parse_argv(t_cmd *cmd, t_token *tokens);

//expander_utils.c >
//string operations helpers.
char	*get_env_value(char **envp, const char *key);
char	*get_env_value_from_substr(char *input, int start, int var_len, char **envp);
char	*join_three_and_free(char *s1, char *s2, char *s3);
int		should_expand(const char *str, int quote_type);

// expander.c
// called after parsing, expanding every thing but not heredocs
char	*replace_variable_in_str(t_exec_path *cmd, char *input, int pos, char **envp);
char	*expand_variables_in_str(t_exec_path *cmd, char *input, char **envp);
void	expand_redirection(t_exec_path *cmd, t_cmd *cmd_list, char **envp);
void	expand_argv(t_exec_path *cmd, char **argv, int *quote_type, char **envp);
void	expand_all_cmds(t_exec_path *cmd, t_cmd *cmd_list, char **envp);

#endif
