#include "parsing.h"
#include <stdio.h>

t_token	*scanner(char *line)
{
	(void) line;
	t_token	*tokens = malloc(sizeof(t_token));
	int len = str_len(line);

	return (tokens);
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
