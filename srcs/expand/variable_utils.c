/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/06/17 22:00:46 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_variable_value(const char *var_name_start, int var_len,
		t_shell *shell)
{
	char	*var_name;
	char	*var_value;

	if (var_len == 1 && var_name_start[0] == '?')
		return (ft_itoa(shell->last_exit_status));
	var_name = ft_strndup(var_name_start, var_len);
	if (!var_name)
		return (NULL);
	var_value = my_getenv(var_name, shell->env_list);
	free(var_name);
	if (!var_value)
		return (ft_strdup(""));
	return (ft_strdup(var_value));
}

int	ft_var_name_len(const char *s)
{
	int	len;

	len = 0;
	while (s[len] && is_valid_var_char(s[len]))
		len++;
	return (len);
}
