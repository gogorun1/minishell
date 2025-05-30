#include "minishell.h"

// 假设 my_getenv(char *key, t_env *env_list) 和 get_last_exit_status_str(int status) 已定义

// Helper to get variable value (either from env or $?)
// Returns a NEWLY ALLOCATED string with the value, or empty string "" if not found/unset.
// Returns NULL on malloc failure.
char	*get_variable_value(const char *var_name_start, int var_len, t_shell *shell)
{
	char *var_name;
	char *var_value;

	if (var_len == 1 && var_name_start[0] == '?') {
		return (ft_itoa(shell->last_exit_status));
	}
	var_name = ft_strndup(var_name_start, var_len);
	if (!var_name)
		return NULL;
	var_value = my_getenv(var_name, shell->env_list);
	free(var_name);
	if (!var_value) { // Variable not found, expand to empty string
		return ft_strdup("");
	}
	return ft_strdup(var_value); // Return a copy as my_getenv returns internal pointer
}

// Helper to determine variable name length (e.g., for $VAR_NAME)
// Assumes str starts at the first char of the var name (e.g., 'V' in $VAR_NAME)
int	ft_var_name_len(const char *s)
{
	int len = 0;
	while (s[len] && is_valid_var_char(s[len]))
		len++;
	return len;
}
