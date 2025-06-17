/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/06/17 22:00:39 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_dollar_sign(t_expand_data *data, const char *str,
		t_shell *shell)
{
	add_literal_part(data, str);
	if (str[data->i + 1] == '?')
		handle_exit_status(data, shell);
	else if (is_valid_var_char(str[data->i + 1]))
		handle_variable(data, str, shell);
	else
		data->i++;
}

static void	finalize_expansion(t_expand_data *data, const char *str)
{
	if (data->start < (int)ft_strlen(str))
		data->result = join_and_free(data->result, (char *)(str + data->start));
}

char	*expand_variables(char *str, t_shell *shell)
{
	t_expand_data	data;

	data.i = 0;
	data.start = 0;
	data.result = ft_strdup("");
	if (!data.result)
		return (NULL);
	while (str[data.i])
	{
		if (str[data.i] == '$' && (str[data.i + 1] == '?'
				|| is_valid_var_char(str[data.i + 1])))
			process_dollar_sign(&data, str, shell);
		else
			data.i++;
	}
	finalize_expansion(&data, str);
	return (data.result);
}
