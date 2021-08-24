/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aclose <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/24 21:59:19 by aclose            #+#    #+#             */
/*   Updated: 2021/08/24 21:59:20 by aclose           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strcdup(char *str, char c)
{
	int		length;
	char	*result;
	int		i;

	length = 1;
	while (str[length] && str[length] != c)
		length++;
	result = malloc(sizeof(char) * (length));
	if (!result)
		return (NULL);
	i = 0;
	while (i < length - 1)
	{
		result[i] = str[i + 1];
		i++;
	}
	result[i] = '\0';
	return (result);
}

char	*ft_strdup_set(char *str, char *set)
{
	int		length;
	char	*result;
	int		i;

	length = 0;
	while (str[length] && !str_has(set, str[length]))
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

void	print_tokens(t_token *tokens)
{
	t_token	*current_token;

	current_token = tokens;
	while (current_token)
	{
		printf("TOKEN: type <%c>, data <%s>\n",
			current_token->type, current_token->data);
		current_token = current_token->next;
	}
}

void	free_tokens_excl_data(t_token *tokens)
{
	t_token	*current_token;

	current_token = tokens;
	if (current_token)
	{
		free_tokens_excl_data(current_token->next);
		free(current_token);
	}
}

void	free_tokens_incl_data(t_token *tokens)
{
	t_token	*current_token;

	current_token = tokens;
	if (current_token)
	{
		free_tokens_incl_data(current_token->next);
		if (current_token->data)
			free(current_token->data);
		free(current_token);
	}
}
