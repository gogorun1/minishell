/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/06/17 22:00:27 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_and_free(char *result, char *to_add)
{
	char	*temp;

	temp = ft_strjoin(result, to_add);
	free(result);
	return (temp);
}

void	add_literal_part(t_expand_data *data, const char *str)
{
	char	*literal;

	if (data->i > data->start)
	{
		literal = ft_strndup(str + data->start, data->i - data->start);
		data->result = join_and_free(data->result, literal);
		free(literal);
	}
}

void	handle_exit_status(t_expand_data *data, t_shell *shell)
{
	char	*status_str;

	status_str = ft_itoa(shell->last_exit_status);
	data->result = join_and_free(data->result, status_str);
	free(status_str);
	data->i += 2;
	data->start = data->i;
}

void	handle_variable(t_expand_data *data, const char *str, t_shell *shell)
{
	int		var_len;
	char	*var_value;

	var_len = ft_var_name_len(str + data->i + 1);
	var_value = get_variable_value(str + data->i + 1, var_len, shell);
	if (var_value)
	{
		data->result = join_and_free(data->result, var_value);
		free(var_value);
	}
	data->i += var_len + 1;
	data->start = data->i;
}
