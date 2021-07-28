#include "parsing.h"

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
			//not the right set of characters , check what characters are valid
			if (token->data[i] == '$')
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
			return (str_dupli(envp[i]));
		i++;
	}
	return (NULL);
}

char	*expand_one_variable(t_token *token, char **envp)
{
	(void)token;
	(void)envp;

	char *name = get_variable_name(token);
	if (!name)
		return (NULL);
	char *value = get_value_from_envp(name, envp);
	return (value);
}

void	expand_variables(char **envp, t_token *tokens)
{
	(void)envp;
	(void)tokens;
	t_token	*current_token;

	current_token = tokens;
	while (current_token)
	{
		if (should_token_be_expanded(current_token))
			current_token->data = expand_one_variable(current_token, envp);
		current_token = current_token->next;
	}
}
