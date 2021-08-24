/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aclose <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/24 21:59:11 by aclose            #+#    #+#             */
/*   Updated: 2021/08/24 21:59:12 by aclose           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_variable_length(char *data)
{
	int	length;

	if (data[0] == '?')
		return (1);
	length = 0;
	while (is_alnum(data[length]) || data[length] == '_')
		length++;
	return (length);
}

int	ft_strncpy(char *dest, char *src, int n)
{
	int	i;

	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (i);
}

static t_bool	is_a_valid_name(char *data)
{
	if (data[0] != '_' && !is_alpha(data[0]) && data[0] != '?')
		return (FALSE);
	return (TRUE);
}

char	*get_variable_name(t_token *token)
{
	int		i;
	char	*name;
	int		length;

	i = 0;
	while (token->data[i] != '$'
		|| is_a_valid_name(token->data + i + 1) == FALSE)
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
			if (token->data[i] == '$' && is_a_valid_name(token->data + i + 1))
				return (TRUE);
			i++;
		}
	}
	return (FALSE);
}
