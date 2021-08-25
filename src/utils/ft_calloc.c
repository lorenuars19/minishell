/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aclose <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 12:58:55 by aclose            #+#    #+#             */
/*   Updated: 2021/08/25 13:40:26 by aclose           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		*((unsigned char *)s + i) = '\0';
		i++;
	}
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	ptr = (void *)malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, nmemb * size);
	return (ptr);
}

void	destroy_strs(char **strs, int i)
{
	int	j;

	j = 0;
	while (j < i)
		free(strs[j++]);
	free(strs);
}

void	sort_envp(char **envp)
{
	t_bool	has_a_swap_occured;
	char	*tmp;
	int		i;

	if (!envp || !envp[0])
		return ;
	has_a_swap_occured = TRUE;
	while (has_a_swap_occured)
	{
		has_a_swap_occured = FALSE;
		i = 0;
		while (envp[i + 1])
		{
			if (str_cmp(envp[i], envp[i + 1]) > 0)
			{
				has_a_swap_occured = TRUE;
				tmp = envp[i];
				envp[i] = envp[i + 1];
				envp[i + 1] = tmp;
			}
			i++;
		}
	}
}
