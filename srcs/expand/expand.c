#include "minishell.h"

static int	expand_var(const char *str, int i, char **result, t_shell *g_shell)
{
	int		var_start;
	int		var_len;
	char	*var_value;
	char	*temp;

	var_start = i + 1;
	var_len = 0;
	while (str[var_start + var_len] && is_valid_var_char(str[var_start
			+ var_len]))
		var_len++;
	var_value = get_variable_value(str + var_start, var_len, g_shell);
	if (var_value)
	{
		temp = ft_strjoin(*result, var_value);
		free(*result);
		*result = temp;
	}
	free(var_value);
	return (var_start + var_len);
}

// Expand variables in a string, replacing $VAR with the value of the variable
// and $ with the last exit status. Returns a newly allocated string.
char	*expand_variables(char *str, t_shell *g_shell)
{
	char *result;
	char *temp = 0;
	int i;
	int start;
	char status_str[12] = {0};

	i = 0;
	start = 0;
	result = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			if (i > start)
			{
				char *s0 = ft_strndup(str + start, i - start);
				temp = ft_strjoin(result, s0);
				free(result);
				free(s0);
				result = temp;
			}
			printf("%d", g_shell->last_exit_status);
			temp = ft_strjoin(result, status_str);
			free(result);
			result = temp;
			i += 2;
			start = i;
		}
		else if (str[i] == '$' && is_valid_var_char(str[i + 1]))
		{
			if (i > start)
			{
				char *s1 = ft_strndup(str + start, i - start);
				temp = ft_strjoin(result, s1);
				free(s1);
				free(result);
				result = temp;
			}
			i = expand_var(str, i, &result, g_shell);
			start = i;
		}
		else
			i++;
	}
	if (start < (int)ft_strlen(str))
	{
		temp = ft_strjoin(result, str + start);
		free(result);
		result = temp;
	}
	return (result);
}