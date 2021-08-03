#include "minishell.h"

t_bool	is_there_unclosed_quotes(char *line)
{
	int	i;
	t_bool	in_quotes;
	char	delimiter;

	i = 0;
	in_quotes = FALSE;
	while (*line)
	{
		if (in_quotes == TRUE && *line == delimiter)
			in_quotes == FALSE;
		else if (in_quotes == FALSE && (*line == '\'' || *line == '\"'))
		{
			in_quotes == TRUE;
			delimiter = *line;
		}
		line++;
	}
	if (in_quotes)
		return (TRUE);
	return (FALSE);
}
