#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>

# include "libutils.h"

# include "parsing.h"
# include "exec.h"


# define HEREDOC_FILENAME "/tmp/.minishell_heredoc.txt"

extern t_info	g_info;

#endif
