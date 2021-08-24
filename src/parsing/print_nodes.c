/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nodes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aclose <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/24 21:57:32 by aclose            #+#    #+#             */
/*   Updated: 2021/08/24 21:57:33 by aclose           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	indent(int spaces)
{
	while (spaces > 0)
	{
		printf(" ");
		spaces--;
	}
}

static void	print_args(t_node *node)
{
	int		i;

	i = 0;
	while (node->args[i])
	{
		if (node->args[i + 1])
			printf("%s, ", node->args[i]);
		else
			printf("%s", node->args[i]);
		i++;
	}
}

static void	print_redirections(t_node *node)
{
	t_redirection	*current_redir;

	if (!node->redirections)
	{
		printf("NONE");
		return ;
	}
	current_redir = node->redirections;
	while (current_redir)
	{
		if (current_redir->next)
			printf("MODE: %c, FILENAME: %s, ",
				current_redir->mode, current_redir->filename);
		else
			printf("MODE: %c, FILENAME: %s",
				current_redir->mode, current_redir->filename);
		current_redir = current_redir->next;
	}
}

void	print_nodes(t_node *nodes, int spaces)
{
	if (nodes->type == COMMAND_NODE)
	{
		indent(spaces);
		printf("COMMAND(");
		printf("ARGS: ");
		print_args(nodes);
		printf("; REDIRECTIONS: ");
		print_redirections(nodes);
		printf(")\n");
	}
	else if (nodes->type == PIPE_NODE)
	{
		indent(spaces);
		printf("PIPE(\n");
		print_nodes(nodes->left, spaces + 2);
		print_nodes(nodes->right, spaces + 2);
		indent(spaces);
		printf(")\n");
	}
}
