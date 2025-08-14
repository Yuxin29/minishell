#ifndef UTILS_H
# define UTILS_H

typedef struct s_cmd	t_cmd;
typedef struct s_token	t_token;
typedef struct s_exec_path	t_exec_path;

// free.c
// free all kind of linked node or single node
void	free_token_list(t_token *token_head);
void	free_redirections(t_cmd *cmd_head);
void	free_cmd_list(t_cmd *cmd_head);
void	free_cmd_node(t_cmd *c);
void	free_t_exec_path(t_exec_path *cmd_and_path);//post_parsing

//str_utils.c
//needed for expander
char	*ft_strjoin_free(char *s1, char *s2);
char	*free_malloc_fail_null(char	*str);//preparsing
int		ft_check_valid_var_name(char c);

//err_msg.c
void	errmsg_set_status(char *msg, t_exec_path *cmd);
void	print_error(const char *arg); //for export
int		perror_return_one(char *str); //for cd
int		errmsg_return_one(char *str); //for cd

#endif
