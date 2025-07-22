#include "minishell.h"

int main(void)
{
	setup_signals();
	repl_loop();
	return (0);
}
