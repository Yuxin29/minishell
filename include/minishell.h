#ifndef MINISHELL_H
# define MINISHELL_H

extern int	g_exit_status; // declare the global exit status

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

/*
External functs allowed .
readline            - get a line from a user with editing       get a line from a user with editing
rl_clear_history    ??
rl_on_new_line      ??
rl_replace_line     ??
rl_redisplay        ??
add_history         ??
printf              ??
malloc, free
write               ??
access             -check user's permissions for a file
open                ??
read                ??
close               - close a file descriptor                  int close(int fd);
fork                -  create a child proces                   pid_t fork(void);
wait, waitpid, wait3, wait4                                     ??
signal              - ANSI C signal handling                    sighandler_t signal(int signum, sighandler_t handler);
sigaction           - examine and change a signal action
sigemptyset         ??
sigaddset           - POSIX signal set operations
kill                 - send a signal to a processq
exit,                — cause the shell to exit              exit [n]
getcwd, chdir, stat, lstat, fstat, unlink, execve,
dup, dup2, pipe, opendir, readdir, closedir,
strerror, perror, isatty, ttyname, ttyslot, ioctl,
getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
tgetnum, tgetstr, tgoto, tputs

#include <unistd.h>         //access, access, close, fork
#include <stdlib.h>         //malloc, free
#include <fcntl.h>
#include <stdio.h>  //readline, printf
#include <string.h>
# include <errno.h>
sigaddset
#include <signal.h>         //signal, sigaction,
# include <sys/wait.h>
*/

#include <setjmp.h> //delete later
extern jmp_buf g_jmpbuf;//delete later

#endif
