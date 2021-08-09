/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_binary.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:49:55 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/09 18:49:56 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_path(t_node *node)
{
	char	*path;
	char	**tab;
	int		i;

	if (node->args && node->args[0] && is_path_executable(node->args[0]))
		return (node->args[0]);
	path = getenv("PATH");
	tab = str_split(path, ":");
	i = 0;
	while (tab && tab[i] && node->args && node->args[0])
	{
		path = str_jointo(tab[i], "/", NULL);
		// path = str_jointo(path, node->args[0], &path);
		path = str_jointo(path, node->args[0], &path);
		if (is_path_executable(path))
		{
			tab_free(&tab);
			return (path);
		}
		free(path);
		i++;
	}
	tab_free(&tab);
	return (NULL);
}

int	exec_binary(t_node *node, char *envp[])
{
	char	*path;
	int		status;

	status = 0;
	path = find_path(node);
	if (!path && node->args)
		return (error_printf(1, "command \"%s\" not found", node->args[0]));
	else if (!path)
		return(error_put(1, "find_path : returned NULL"));

//TODO remove debug
dprintf(2, ">\n \033[32;1mexecve\033[0m(path \"%s\", node->args <%p>, envp <%p>)\n<", path, node->args, envp);

	if (setup_signals(DEFER_SIGNAL))
			return (error_sys_put("setup_signals"));
	status = execve(path, node->args, envp);
	free(path);
	return (status);
}
