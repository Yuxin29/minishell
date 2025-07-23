#include "parsing.h"

// parser.c
// syntax check (pipes, etc.)
int check_syntax(t_token *tokens);
// build command structures
t_cmd *build_command_list(t_token *tokens);
// handle redirections
void parse_redirections(t_cmd *cmd, t_token **tokens);
// group commands by pipe
t_cmd *group_by_pipes(t_token *tokens);
