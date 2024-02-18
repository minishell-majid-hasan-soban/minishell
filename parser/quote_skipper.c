#include <stdio.h>
#include <string.h>
#include "../includes/parser.h"


int	skip_quotes_work(char **dest, char **str, char quote)
{
	(*str)++;
	if (!is_in_str(*str, quote))
	{
		printf("minishell: syntax error, expected `%c`\n", quote);
		return (-1);
	}
	while (**str != quote)
	{
		*(*dest) = *(*str);
		(*str)++;
		(*dest)++;
	}
	(*str)++;
	return (1);
}

char *skip_quotes(char *str)
{
	int i = 0;
	int j = 0;
	char *result = ft_calloc(ft_strlen(str) + 2, 1);
	char *tmp_str;
	char *tmp_result;
	while(str[i] && !is_space(str[i]))
	{
		if(str[i] == '\'' || str[i] == '"')
		{
			tmp_str = str + i;
			tmp_result = result + j;
			skip_quotes_work(&tmp_result, &tmp_str, str[i]);
			i += tmp_str - (str + i);
			j += tmp_result - (result + j);
			continue;
		}
		result[j] = str[i];
		printf("%c\n", str[i]);
		i++;
		j++;
	}
	result[j] = 0;
	return result;
}

