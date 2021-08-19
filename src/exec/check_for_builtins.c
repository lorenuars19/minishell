/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_for_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:49:52 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/19 16:45:26 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_dummmy(char *argv[], char *envp[])
{
	(void)envp;
	(void)argv;
	return (0);
}

static t_builtin_f get_builtin(int index)
{
	static t_builtin_f	builtins[] = {
		builtin_echo,
		builtin_cd,
		builtin_pwd,
		builtin_export,
		builtin_unset,
		builtin_env,
		builtin_empty
		};
	if (index >= 0 && index < BUILTIN_MAX)
	{
		return(builtins[index]);
	}
	return (builtin_empty);
}

void	check_for_builtins(t_exdat *ed, t_node *node)
{
	int			i;
	static char	*builtins[] = {
		"echo",
		"cd",
		"pwd",
		"export",
		"unset",
		"env",
		"exit",
		NULL};
	i = 0;
	while (node->args && node->args[0] && builtins[i] \
		&& str_cmp(node->args[0], builtins[i]))
		i++;

	if (ed->is_pipe == FALSE && (i == BUILTIN_CD || i == BUILTIN_EXPORT
			|| i == BUILTIN_UNSET || i == BUILTIN_EXIT))
		ed->is_fork = FALSE;

	ed->f_to_call = get_builtin(i);

	if (i >= 0 && i < BUILTIN_MAX)
		ed->is_builtin = TRUE;
}
