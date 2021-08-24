/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aclose <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/24 21:59:16 by aclose            #+#    #+#             */
/*   Updated: 2021/08/24 21:59:17 by aclose           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_blank_tokens(t_token **tokens)
{
	while (*tokens && (*tokens)->type == T_BLANK)
		(*tokens) = (*tokens)->next;
}

t_bool	token_contains_a_string(t_token *token)
{
	t_token_type	type;

	if (!token)
		return (FALSE);
	type = token->type;
	if (type == T_GENERAL || type == T_DQUOTE || type == T_SQUOTE)
		return (TRUE);
	return (FALSE);
}

t_bool	line_contains_only_blanks(char *line)
{
	while (*line)
	{
		if (*line != ' ' && *line != '\t')
			return (FALSE);
		line++;
	}
	return (TRUE);
}
