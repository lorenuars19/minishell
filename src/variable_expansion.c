#include "parsing.h"
#include <stdlib.h>

t_bool	ft_isspace(char c)
{
	return (str_has(SPACE_STRING, c));
}

int	get_variable_length(char *data)
{
	int	length;

	if (is_digit(data[0]))
		return (0);
	length = 0;
	while (is_alnum(data[length]) || data[length] == '_')
		length++;
	return (length);
}

static	void	ft_strncpy(char *dest, char *src, int n)
{
	int	i;

	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

char	*get_variable_name(t_token *token)
{
	int		i;
	char	*name;
	int		length;

	i = 0;
	while (token->data[i] != '$')
		i++;
	length = get_variable_length(token->data + i + 1);
	if (length == 0)
		return (NULL);
	name = ft_calloc(length + 1, sizeof(char));
	if (!name)
		return (NULL);
	ft_strncpy(name, token->data + i + 1, length);
	return (name);
}

static t_bool	is_a_valid_name(char *data)
{
	if (data[0] == '$' && (data[1] == '_' || is_alnum(data[1])))
		return (TRUE);
	return (FALSE);
}

t_bool	should_token_be_expanded(t_token *token)
{
	t_token_type	type;
	int				i;

	type = token->type;
	if (type == T_GENERAL || type == T_DQUOTE)
	{
		i = 0;
		while (token->data[i])
		{
			if (token->data[i] == '$' && is_a_valid_name(token->data + i))
				return (TRUE);
			i++;
		}
	}
	return (FALSE);
}

char	*get_value_from_envp(char *name, char **envp)
{
	int		i;
	size_t	length;

	i = 0;
	length = str_len(name);
	while (envp[i])
	{
		if (str_cmp_n(envp[i], name, length) == 0 && envp[i][length] == '=')
			return (str_dupli(&(envp[i][length + 1])));
		i++;
	}
	return (str_dupli(""));
}

char	*replace_variable_with_its_value(t_token *token, char *value, char *name)
{
	(void)token;
	(void)value;

	char	*result;
	int		i;
	int		length;

	length = str_len(value) - str_len(name) + str_len(token->data);
	result = ft_calloc(length, sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	while (TRUE)
	{
		if (token->data[i] == '$' && token->data[i + 1] == name[0])
			break ;
		result[i] = token->data[i];
		i++;
	}
	ft_strncpy(result + i, value, str_len(value));
	while (i + (int)str_len(value) < length)
	{
		result[i + str_len(value)] = token->data[i + str_len(name) + 1];
		i++;
	}
	return (result);
}

char	*expand_one_variable(t_token *token, char **envp)
{
	char	*name;
	char	*value;
	char	*expanded_token;

	name = get_variable_name(token);
	if (!name)
		return (NULL);
	value = get_value_from_envp(name, envp);
	if (!value)
	{
		free(name);
		return (NULL);
	}
	expanded_token = replace_variable_with_its_value(token, value, name);
	free(name);
	free(value);
	return (expanded_token);
}

char	*expand_in_one_token(t_token *token, char **envp)
{
	char	*expanded_token;

	while (should_token_be_expanded(token))
	{
		expanded_token = expand_one_variable(token, envp);
		if (!expanded_token)
			return (NULL);
		free(token->data);
		token->data = expanded_token;
	}
	return (token->data);
}

void	expand_variables(char **envp, t_token *tokens)
{
	t_token	*current_token;

	current_token = tokens;
	while (current_token)
	{
		if (should_token_be_expanded(current_token))
		{
			expand_in_one_token(current_token, envp);
			//TODO check value of current_token->data, if NULL handle error
		}
		current_token = current_token->next;
	}
}
