#include "minishell.h"

static int ft_length(int n)
{
	long i;
	int len;

	if (n == 0)
		return (1);
	i = n;
	len = 0;
	if (i < 0)
	{
		len++;
		i = -i;
	}
	while (i != 0)
	{
		i /= 10;
		len++;
	}
	return (len);
}

char *ft_itoa(int n)
{
	char *res;
	unsigned int i;
	int is_neg;
	int len;

	if (n < 0)
		is_neg = 1;
	else
		is_neg = 0;
	i = n;
	if (n < 0)
		i = -n;
	len = ft_length(n);
	res = ft_calloc(len + 1, sizeof(char));
	if (res == NULL)
		return (NULL);
	while (i != 0 || (n == 0 && len == 1))
	{
		res[len - 1] = i % 10 + '0';
		i /= 10;
		len--;
	}
	if (is_neg)
		res[0] = '-';
	return (res);
}
