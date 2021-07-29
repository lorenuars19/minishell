#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

#include "libutils.h"

typedef	enum	e_node_type
{
	COMMAND_NODE,
	PIPE_NODE
}	t_node_type;

typedef	struct s_node	t_node;

struct s_node
{
	t_node_type	type;
	char	**args; //place
	t_node	*right;	//in case of a pipe node
	t_node	*left; //in case of a pipe node
};

#endif
