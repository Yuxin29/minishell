#ifndef MINISHELL_H
# define MINISHELL_H

//----------------------header files and global variable-------------------//
# include "../libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <signal.h>
# include <sys/types.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <errno.h>
# include <sys/stat.h>

extern volatile sig_atomic_t	g_signal;

//---------------------------MACROS part----------------------------------//
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
# define CD_ERR_MSG \
	"cd: error retrieving current directory: getcwd: cannot \
	access parent directories"
# define SYNTAX_ERR_AMBIGUOUS \
	"minishell: ambiguous redirect"

//common shell cmd line max length
# define LINE_SIZE 8192
//PATH_MAX is 4096 on Linux by convention and practicality.
# define PATH_MAX 4096
//yuxin added for exit, stoll based on long long // probably not needed
# define LLONG_MAX_STR "9223372036854775807"
# define LLONG_MIN_STR "-9223372036854775808"

//---------------------------struct part--------------------------------//
// 	T_WORD,			0			string			a cmd, a arguement
// 	T_PIPE,			1    |      pipe
// 	T_REDIRECT_IN,	2     <     input   		Reads from a file
// 	T_REDIRECT_OUT,	3     >     output  		Writes to a file, replacing.
// 	T_APPEND,       4     >>    output append   writes to a file, appends.
// 	T_HEREDOC,      5    <<   	heredoc input   Feeds inline text as stdin.
typedef enum s_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIRECT_IN,
	T_REDIRECT_OUT,
	T_APPEND,
	T_HEREDOC,
}	t_token_type;

// quote_type: 0 no qt; 1 single qt; 2: double qt, 3: QUOTE_DOLLAR_DOUBLE
typedef struct s_token
{
	char			*str;
	t_token_type	t_type;
	int				quote_type;
	struct s_token	*next;
}	t_token;

// linked list of redirections
// type: redirection type:  inherete from t_token: 2 - 5
// quoted: 0 no qt; 1 single qt; 2: double qt, 3: QUOTE_DOLLAR_DOUBLE
// heredoc_delim: null when 2 - 4,
typedef struct s_redir
{
	char			*file;
	int				type;
	int				quoted;
	char			*heredoc_delim;
	int				is_ambiguous; //yuxin added, cat <$AA
	struct s_redir	*next;
}	t_redir;

// quote_type;
// inherete from t_token: 0, 1, 2, 3 removed when removing quotes
// cmd_path: null at parsing phase
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

//exec
// *whole_cmd;	whole cmd_list
// **envp:  	the copy one
typedef struct s_exec_path
{
	t_cmd	*whole_cmd;
	char	**envp;
	int		exit_status;
	int		orig_std[3]; //modify 0818
}	t_exec_path;

//for inv
typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}	t_env;

//pipes
typedef struct s_pipe_ex
{
	int		prev_pipe;
	int		pipefd[2];
	pid_t	last_pid;
}	t_pipe_ex;

//---------------------------main helper----------------------------------//
int			setup_command_paths(t_exec_path *exec_cmd, t_env **env_list);
int			run_command(t_exec_path *exec_cmd, t_env **env_list);
int			parse_and_expand(t_exec_path *exec_cmd, char *expanded_line,\
	t_env **env_list);
void		handle_line(char *line, t_env **env_list, t_exec_path *exec_cmd);
void		minishell_loop(t_env **env_list);

//---------------------------parsing part----------------------------------//

//pre_syntax_checking.c
int			check_line_syntax(char *raw_line);

//preexpander_utils.c	5/5
int			skip_copy(char *raw_line, int idx[2], char *res, int quotes[2]);
int			handle_quotes(char c, int quotes[2], char *res, int *j);
int			handle_heredoc_skip(char *raw_line, int ids[2], char *res);
int			handle_dollar_dquote(char *raw_line, int ids[2], char *res);
int			handle_exit_status(char *raw_line, int ids[2], char *res,\
	t_exec_path *cmd);

//preexpander.c		5/5
int			try_expand_env_var(char *raw_line, int idx[2], char *res,\
	t_exec_path *cmd);
void		expand_loop(char *raw_line, char *res, int idx[2],\
	t_exec_path *cmd);
char		*pre_expand_line(t_exec_path *cmd, char *raw_line);

// lex_utils.c	5/5
// all used outside of the file
// ATTENTION: empty token list is alloweed. when its leagal, status = 0
// when it is empty but status == 1, it is from malloc failure
// when it is empty but status == 2, it is from symtax error
//void		precheck_special_chars_rawline(char *line, t_exec_path *cmd);
//void		check_raw_line_syntax(char *raw_line, t_exec_path *cmd);
int			precheck_special_chars_rawline(char *line);
int			check_raw_line_syntax(char *raw_line);
void		get_token_type(t_token *token);
char		*get_unquoted_part(char *s, int *i, t_exec_path *cmd);
char		*get_quoted_part(char *s, int *i, t_exec_path *cmd);

// lexing_w_token.c		5/5
t_token		*build_word_token(char *line, int *i, t_exec_path *cmd);

// lex.c				4/5
// get a raw line and change it to a linked list of minimal unit(tokens)
t_token		*build_operator_token(char *line, int *i, t_exec_path *cmd);
t_token		*get_token_list(t_exec_path *cmd, char *raw_line);

// parsing_redir.c	5/5
t_token		*parse_redirections(t_cmd *cmd, t_token *tokens,\
	t_exec_path *exec_cmd);

// parsing_argv.c	5/5
t_token		*parse_argv(t_cmd *cmd, t_token *tokens);

// parsing_one_cmd.c	4/5
// 3 used out of this .c
// ATTENTION: empty token list is alloweed. when its leagal, status = 0
// when it is empty but status == 2, it is from symtax error
t_token		*get_one_new_cmd(t_token *token_head, t_cmd *cmd_current,\
	t_exec_path *exec_cmd);

// parsing_to_cmd_list.c	3/5
t_cmd		*build_command_list(t_exec_path *cmd, t_token *token_head);
//---------------------------exec part------------------------------------//
//execute builtins
int			is_builtin(char *cmd);
int			execute_builtin_cmd(char **argv, t_env **env,\
	t_exec_path *exec_cmd);
void		run_builtin_with_redir(t_exec_path *exec_cmd, t_env **env_list);
void		restore_stdio(int stdin_fd, int stdout_fd);
//exec_single
void		execute_single_cmd(t_exec_path *cmd, t_env **env_list);
void		print_error_and_exit(t_cmd *cmd, t_exec_path *exec_cmd,\
	t_env **env_list);
void		precheck_path_or_exit(char *path, t_exec_path *cmd,\
	t_env **env_list);
//exec_pipline
void		execute_pipeline(t_exec_path *exec_cmd, t_env **env_list);
//exec_pipeline_utils
int			create_pipe_or_exit(t_exec_path *exec_cmd, t_pipe_ex *pinfo);
int			set_up_stdin(t_pipe_ex *pinfo, t_exec_path *cmd, t_env **env_list);
int			set_up_stdout(t_cmd *cmd, t_pipe_ex *pinfo, t_exec_path *exec_cmd,\
	t_env **env_list);
void		wait_exit(t_exec_path *exec_cmd, pid_t last_pid);
//exec_utils
void		handle_execve_or_exit_inchild(t_exec_path *exec_cmd,\
	t_cmd *cmd, t_env **env_list);
char		*cleanup_heredoc(int fd, int saved_stdin, char *tmp_file,\
	char *err_msg);
void		free_all_and_exit(t_exec_path *cmd, t_env **env_list, int status);
void		free_all_and_exit_pipe(t_exec_path *cmd, t_env **env_list,\
	int status, t_pipe_ex *pinfo);
//redirections
int			check_and_apply_redirections(t_cmd *cmd);
//heredoc
char		*creat_heredoc_file(char *delim, int quoted, t_exec_path *cmd);
//get_path
char		*get_env(t_env *env, char *key);
char		*get_cmd_path(char *cmd, t_env *env_list, t_exec_path *exec_cmd);
//---------------------------built_in part--------------------------------//
//7 builtin cmds:
//builtins_opera1
int			ft_echo(char **argv);
int			ft_exit(char **argv, t_exec_path *exec_cmd, t_env **env_list);
int			ft_pwd(t_env **env);
//builtins_opera2
int			ft_cd(char **argv, t_env **env);
int			ft_env(t_env **env);
int			ft_unset(char **argv, t_env **env);
//builtins_opera3
int			ft_export(char **argv, t_env **env);
//builtins_utils
void		set_env(t_env **env, char *key, char *value);
int			ft_isnot_numeric(char *str);
int			env_count(t_env *env);
void		sort_copy_list(t_env **copy_list, int size);
//---------------------------env part---------------------------------------//
// NOTES
// in minishell,
// we might change the elements of the original env(impletment unset, export),
// so we have to copy the oringal env, and change them in copy version
// first step is converting every envp[i] to a signal node,
// and connect these nodes to a linked list,
// in which we can add or delete env elements more easily than in array,
// after modifying the env list, we should convert linked list to array

//get env list
t_env		*env_new_node(char *key, char *value);
t_env		*env_list_init(char **envp);
void		free_env_list(t_env **head);

//env list to envp
// second used in preexpander
char		**env_list_to_envp(t_env *head);
char		*get_env_value(char **envp, const char *key);
//---------------------------signal part-------------------------------------//
//signals.c
// 2 + 2 static inside
void		signal_init(void);
void		signal_default(void);
// signal_utils.c
void		signal_heredoc(void);
void		signal_ignore(void);
int			hd_is_interrupted(void);

//---------------------------utils part---------------------------------//
// free.c
// free all kind of linked node or single node
void		free_token_list(t_token *token_head);
void		free_redirections(t_cmd *cmd_head);
void		free_cmd_list(t_cmd *cmd_head);
void		free_cmd_node(t_cmd *c);
void		free_t_exec_path(t_exec_path *cmd_and_path);

// str_utils.c
// needed for expander and mallac failure halding in parsing
int			ft_check_valid_var_name(char c);
char		*ft_strjoin_free(char *s1, char *s2);
char		*join_three_and_free(char *s1, char *s2, char *s3);
char		*free_malloc_fail_null(char	*str);
char		*free_malloc_fail_null_status(char *str, t_exec_path *cmd);

//err_msg.c
//used in builtin(export and cd), syntax check(stat 2) and main
void		print_error(char *str1, const char *arg, char *str3);
int			errmsg_return_nbr(char *str, int i, int nbr);
void		errmsg_set_status(char *msg, t_exec_path *cmd);
int			handle_failure(t_exec_path *exec_cmd, t_env **env_list, char *msg);
int			var_name_len(const char *str);

#endif
