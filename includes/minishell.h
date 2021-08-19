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

# define HEREDOC_PATH "/tmp/.minishell_heredoc_tmp"

typedef struct s_shell_data
{
	char	*line;

	t_token *tokens;
	t_node	*nodes;
}	t_sd;

typedef struct s_info
{
	char	**envp;
	t_node	*nodes;
	int		last_exit_status;
}	t_info;

extern t_info g_info;

#endif
