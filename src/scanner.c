#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>

t_token_type	get_char_type(char c)
{
	static char	*special_char = "|\'\" ";

	// printf("<%s>\n", special_char);
	if (str_has(special_char, c))
		return (c);
	return (T_GENERAL);
}

char	*ft_strcdup(char *str, char c)
{
	int		length;
	char	*result;
	int		i;

	length = 1;
	while (str[length] && str[length] != c)
		length++;
	if (str[length] == c)
		length++;
	result = malloc(sizeof(char) * (length + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < length)
	{
		result[i] = str[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

int	get_quote_token(char *line, t_token *token, int *index)
{
	token->type = get_char_type(*line);
	token->data = ft_strcdup(line, token->type);
	(*index)++;
	return (0);
}

t_token	*scanner(char *line)
{
	t_token*	current_token = malloc(sizeof(t_token));
	int len = str_len(line);
	int i = 0;
	t_state state = STATE_GENERAL;
	(void)len;
	(void)state;
	char	c;

	while (i < len)
	{
		c = line[i];
		t_token_type type = get_char_type(c);
		if (type == T_DQUOTE || type == T_SQUOTE)
			get_quote_token(line + i, current_token, &i);
		i++;
	}
	return (current_token);
}

void	print_tokens(t_token *tokens)
{
	t_token	*current_token;

	current_token = tokens;
	while (current_token)
	{
		printf("type: %c, data: <%s>\n", current_token->type, current_token->data);
		current_token = current_token->next;
	}
}
