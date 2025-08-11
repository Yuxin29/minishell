#ifndef MINISHELL_H
# define MINISHELL_H

# include "parsing.h"
# include "builtins.h"
# include "exec.h"
# include "utils.h"
# include "signals.h"

# include "../libft/libft.h"
# include <unistd.h>         //access, access, close, fork
# include <stdlib.h>         //malloc, free
# include <fcntl.h>
# include <stdio.h>                 //readline
# include <signal.h>
# include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>
# include <sys/wait.h>
# include <errno.h> //？？

extern volatile sig_atomic_t g_signal;

typedef struct s_cmd t_cmd;
typedef struct s_token t_token;
typedef struct s_exec_path t_exec_path;

#endif
