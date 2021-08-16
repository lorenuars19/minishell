#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <signal.h>

# include "libutils.h"

# include "parsing.h"
# include "exec.h"


# define HEREDOC_FILENAME "/tmp/.minishell_heredoc.txt"

typedef struct s_shell_data
{
	char	***envp_addr;

	char	*line;

	int		status;

	t_token *tokens;
	t_node	*nodes;
}	t_sd;


#endif
