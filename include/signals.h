#ifndef SIGNALS_H
# define SIGNALS_H

void	signal_init(void);
void	signal_default(void);
void	signal_heredoc(void);
void	signal_ignore(void);
int		hd_is_interrupted(void);

#endif
