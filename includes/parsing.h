#ifndef PARSING_H
# define PARSING_H

#include "libutils.h"

# define MALLOC_ERROR -1
# define SPECIAL_CHARS "|\'\" <>\t"
typedef	enum e_token_type
{
	T_GENERAL = 'g',
	T_SPACE = ' ',
	T_TAB = '\t',
	T_BLANK = 'b',
	T_PIPE = '|',
	T_SQUOTE = '\'',
	T_DQUOTE = '\"',
	T_GREATER = '>',
	T_SMALLER = '<'
}	t_token_type;

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
char *ft_strdup_set(char *str, char *set);
void free_tokens(t_token *tokens);
void *ft_calloc(size_t nmemb, size_t size);

#endif
