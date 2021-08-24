#include "minishell.h"

int	get_quote_token(char *line, t_token *token, int *index)
{
	token->type = get_token_type(*line);
	token->data = ft_strcdup(line, token->type);
	if (token->data == NULL)
		return (MALLOC_ERROR);
	(*index) += str_len(token->data) + 2;
	return (0);
}

int	get_general_token(char *line, t_token *token, int *index)
{
	token->type = T_GENERAL;
	token->data = ft_strdup_set(line, SPECIAL_CHARS);
	if (token->data == NULL)
		return (MALLOC_ERROR);
	(*index) += str_len(token->data);
	return (0);
}

int	get_blank_token(char *line, t_token *token, int *index)
{
	token->type = T_BLANK;
	token->data = NULL;
	while (*line == ' ' || *line == '\t')
	{
		line++;
		(*index)++;
	}
	return (0);
}

int	get_pipe_token(char *line, t_token *token, int *index)
{
	(void)line;
	token->type = T_PIPE;
	token->data = NULL;
	(*index)++;
	return (0);
}

int	get_redirection_token(char *line, t_token *token, int *index)
{
	if (*line == '<' && *(line + 1) == '<')
	{
		token->type = T_HEREDOC;
		*index += 2;
	}
	else if (*line == '>' && *(line + 1) == '>')
	{
		token->type = T_APPEND;
		*index += 2;
	}
	else
	{
		token->type = get_token_type(*line);
		(*index)++;
	}
	return (0);
}
