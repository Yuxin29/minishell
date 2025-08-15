#ifndef PARSING_H
# define PARSING_H

//-----------------------------------------------------------------------------------

//Needed from exec.h
typedef struct s_exec_path	t_exec_path;

// syntax error ms macors
# define SYNTAX_ERR_PIPE \
	"minishell: syntax error near unexpected token `|'"
# define SYNTAX_ERR_REDIR_DOUBLE \
	"minishell: syntax error: near unexpected token redirections"
# define SYNTAX_ERR_REDIR_FILE_MISSING \
	"minishell: syntax error: missing filename or delimiter after redirection"
# define SYNTAX_ERR_SPECIAL_CHARS \
	"minishell: syntax error: special characters"
# define SYNTAX_ERR_QUOTES \
	"minishell: syntax error: unclosed quotes"

//common shell cmd line length
# define BUF_SIZE 8192

//needed for expand, EMPTY=
# define EMPTY ""

// 	T_WORD,			0			string			a cmd, a arguement
// 	T_PIPE,			1    |      pipe
// 	T_REDIRECT_IN,	2     <     input   		Reads from a file
// 	T_REDIRECT_OUT,	3     >     output  		Writes to a file, replacing.
// 	T_APPEND,       4     >>    output append   writes to a file, appends.
// 	T_HEREDOC,      5    <<   	heredoc input   Feeds inline text as stdin.
typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIRECT_IN,
	T_REDIRECT_OUT,
	T_APPEND,
	T_HEREDOC,
}	t_token_type;

// quote_type		
// 0: no quote; 1: single quote; 2: double quote, 3: QUOTE_DOLLAR_DOUBLE
typedef struct s_token
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

// quote_type; inherete from t_token: 0, 1, 2, 3 removed when removing quotes
// ATTENTION: argv is empty is allowed
// case 1: < infile
// case 2: "   " or $EMPTY ----> argv = NULL / argv[0] = NULL
typedef struct s_cmd
{
	char			**argv;
	int				*quote_type;
	t_redir			*redirections;
	char			*cmd_path;
	struct s_cmd	*next;
}	t_cmd;

//preexpander_utils.c
int		var_name_len(const char *str);
int		handle_quotes(char c, int quotes[2], char *res, int *j);
int		handle_heredoc_skip(char *raw_line, int ids[2], int *skip_expand, char *res);
int		handle_dollar_dquote(char *raw_line, int ids[2], char *res);
int		handle_exit_status(char *raw_line, int ids[2], char *res, t_exec_path *cmd);

//preexpander.c
void	append_to_res(char *res, int *res_idx, const char *val);
int		try_expand_env_var(char *raw_line, int idx[2], char *res, char **envp);
int		skip_copy(char *raw_line, int idx[2], char *res, int quotes[2]);
void	expand_loop(char *raw_line, char *res, int idx[2], t_exec_path *cmd);
char	*pre_expand_line(t_exec_path *cmd, char *raw_line);

// lex_utils.c
// ATTENTION: empty token list is alloweed. when its leagal, status = 0
// when it is empty but status == 2, it is from symtax error 
void	precheck_special_chars_rawline(char *line, t_exec_path *cmd);
void	check_raw_line_syntax(char *raw_line, t_exec_path *cmd);
void	get_token_type(t_token *token);
char	*get_unquoted_part(char *s, int *i);
char	*get_quoted_part(char *s, int *i);

// lexing_w_token.c
char	*get_part(char *line, int *i, char	*part_quote);
t_token	*malloc_and_set_token(char *temp, int q);
char	*append_next_part(char *temp, char *line, int *i, char *part_quote);
t_token	*build_word_token(char *line, int *i);

// lex.c
// get a raw line and change it to a linked list of minimal unit(tokens)
t_token	*get_token_list(t_exec_path *cmd, char *raw_line);
t_token	*tokenize_loop(char *raw_line);
t_token	*build_operator_token(char *line, int *i);
t_token	*build_token_from_next_word(char *line, int *i);

// parser_utils.c
// ATTENTION: empty token list is alloweed. when its leagal, status = 0
// when it is empty but status == 2, it is from symtax error 
void	check_token_syntax(t_token *token_head, t_exec_path *cmd);
int		count_argv(t_token *start);
t_token	*parse_if_no_argv(t_cmd *cmd_current, t_token *token_head);
t_token	*loop_to_next(t_token *token_head);
t_token	*get_one_new_cmd(t_token *token_head, t_cmd *cmd_current);

// parser.c
// change token_list to command list and free the original tokens
// - Taking the token list and Understanding the structure of the command,
// - Grouping tokens into command nodes, pipes, redirections, etc,
// - Building a linked list of command objects (t_cmd)
t_redir	*create_redir_node(t_token *redir_tok, t_token *file_tok);
t_token	*get_one_redirection(t_cmd *cmd, t_token *tokens);
t_token	*parse_redirections(t_cmd *cmd, t_token *tokens);
int		malloc_for_agrv(t_cmd *cmd, t_token *tokens);
t_token	*parse_argv(t_cmd *cmd, t_token *tokens);

// parsing_to_cmd_list.c
int		check_new_cmd(t_exec_path *cmd, t_token *token, t_cmd *cmd1, t_cmd *cmd2);
t_cmd	*malloc_for_new_cmd(t_cmd *cmd_head);
t_cmd	*build_command_list(t_exec_path *cmd, t_token *token_head);

//expander_utils.c >
//string operations helpers.
char	*get_env_value(char **envp, const char *key);
char	*get_env_value_from_substr(char *input, int start, int var_len, char **envp);
char	*join_three_and_free(char *s1, char *s2, char *s3);
int		should_expand(const char *str, int quote_type);

// expander.c
// called after parsing, expanding every thing but not heredocs
char	*replace_variable(t_exec_path *cmd, char *input, int pos, char **envp);
char	*expand_variables_in_str(t_exec_path *cmd, char *input, char **envp);
void	expand_redirection(t_exec_path *cmd, t_cmd *cmd_list, char **envp);
void	expand_argv(t_exec_path *cmd, char **argv, int *quote_type, char **envp);
void	expand_all_cmds(t_exec_path *cmd, t_cmd *cmd_list, char **envp);

//-----------------------------------------------------------------------------------

#endif

