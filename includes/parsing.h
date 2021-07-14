#ifndef PARSING_H
# define PARSING_H

#include "libutils.h"

typedef	enum e_token_type
{
	GENERAL = 'g',
	SPACE = ' ',
	PIPE = '|',
	SQUOTE = '\'',
	DQUOTE = '\"',
	GREATER = '>',
	LESSER = '<',
	EOL = 'e'
}	t_token_type;

typedef	struct s_token
{
	char			*data;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

t_token	*scanner(char *line);

#endif
