#ifndef MINISHELL_H
# define MINISHELL_H

# include "parsing.h"
# include "builtins.h"
# include "exec.h"
# include "utils.h"
# include "../libft/libft.h"
# include "repl.h"
# include "signals.h"

# include <unistd.h>         //access, access, close, fork
# include <stdlib.h>         //malloc, free
# include <fcntl.h>
# include <stdio.h>                 //readline

extern volatile sig_atomic_t g_sig_num;
#include <setjmp.h> //delete later
extern jmp_buf g_jmpbuf;//delete later

#endif
