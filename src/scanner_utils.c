#include "parsing.h"
#include <stdlib.h>
#include <stdio.h>

static void ft_bzero(void *s, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		*((unsigned char *)s + i) = '\0';
		i++;
	}
}

void *ft_calloc(size_t nmemb, size_t size)
{
	void *ptr;

	ptr = (void *)malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, nmemb * size);
	return (ptr);
}

char *ft_strcdup(char *str, char c)
{
	int length;
	char *result;
	int i;

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

char *ft_strdup_set(char *str, char *set)
{
	int length;
	char *result;
	int i;

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

void print_tokens(t_token *tokens)
{
	t_token *current_token;

	current_token = tokens;
	while (current_token)
	{
		printf("type: <%c>, data: <%s>\n", current_token->type, current_token->data);
		current_token = current_token->next;
	}
}

void free_tokens_without_data(t_token *tokens)
{
	t_token *current_token;

	current_token = tokens;
	if (current_token)
	{
		free_tokens_without_data(current_token->next);
		free(current_token);
	}
}

void	free_tokens_with_data(t_token *tokens)
{
	t_token *current_token;

	current_token = tokens;
	if (current_token)
	{
		free_tokens_with_data(current_token->next);
		if (current_token->data)
			free(current_token->data);
		free(current_token);
	}
}
