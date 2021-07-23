#ifndef PARSING_H
# define PARSING_H

#include "libutils.h"

# define MALLOC_ERROR -1
typedef	enum e_token_type
{
	T_GENERAL = 'g',
	T_SPACE = ' ',
	T_PIPE = '|',
	T_SQUOTE = '\'',
	T_DQUOTE = '\"',
	T_GREATER = '>',
	T_LESSER = '<',
	T_EOL = 'e'
}	t_token_type;

typedef	enum e_state
{
	STATE_GENERAL = 'g',
	STATE_SQUOTE = '\'',
	STATE_DQUOTE = '\"'
} t_state;

typedef	struct s_token
{
	char			*data;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

t_token	*scanner(char *line);
t_token_type	get_char_type(char c);
void	print_tokens(t_token *tokens);
char *ft_strcdup(char *str, char c);

#endif
