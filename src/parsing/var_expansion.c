/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aclose <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/24 21:59:08 by aclose            #+#    #+#             */
/*   Updated: 2021/08/24 21:59:09 by aclose           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace_variable_by_its_value(t_token *token, char *value, char *name)
{
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
	if (ft_strncmp(name, "?", 1) == 0)
		value = ft_itoa(g_shell.last_exit_status);
	else
		value = get_value_from_envp(name, envp);
	if (!value)
	{
		free(name);
		return (NULL);
	}
	expanded_token = replace_variable_by_its_value(token, value, name);
	free(name);
	free(value);
	return (expanded_token);
}

int	expand_in_one_token(t_token *token, char **envp)
{
	char	*expanded_token;

	while (should_token_be_expanded(token))
	{
		expanded_token = expand_one_variable(token, envp);
		if (!expanded_token)
			return (-1);
		free(token->data);
		token->data = expanded_token;
	}
	return (0);
}

static void	skip_delimiter(t_token **tokens)
{
	t_token_type	type;
	t_token			*first_delimiter_token;

	type = (*tokens)->type;
	if (type == T_HEREDOC)
		*tokens = (*tokens)->next;
	skip_blank_tokens(tokens);
	type = (*tokens)->type;
	first_delimiter_token = *tokens;
	while ((*tokens) && (type == T_GENERAL
			|| type == T_DQUOTE || type == T_SQUOTE))
	{
		if (type == T_DQUOTE || type == T_SQUOTE)
			first_delimiter_token->type = type;
		*tokens = (*tokens)->next;
		if (*tokens)
			type = (*tokens)->type;
	}
}

int	expand_variables(char **envp, t_token *tokens)
{
	t_token	*current_token;

	current_token = tokens;
	while (current_token)
	{
		if (should_token_be_expanded(current_token))
		{
			if (expand_in_one_token(current_token, envp) != 0)
				return (-1);
		}
		if (current_token->type == T_HEREDOC)
		{
			skip_delimiter(&current_token);
			continue ;
		}
		current_token = current_token->next;
	}
	return (0);
}
