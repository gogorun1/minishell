/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:27:14 by wding             #+#    #+#             */
/*   Updated: 2025/06/17 22:27:16 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_empty_input(char *input)
{
	if (!input)
	{
		printf("exit\n");
		return (1);
	}
	if (input[0] == '\0')
	{
		free(input);
		return (2);
	}
	return (0);
}

static int	process_tokens(char *input, t_shell *shell, t_exec_data *data)
{
	data->tokens = tokenizer(input, shell);
	if (!data->tokens)
	{
		shell->last_exit_status = 1;
		return (1);
	}
	return (0);
}

static int	process_ast(t_exec_data *data, t_shell *shell)
{
	data->ast = parse(data->tokens);
	if (!data->ast)
	{
		if (g_signal_status == 130)
		{
			shell->last_exit_status = 130;
			g_signal_status = 0;
		}
		else
			shell->last_exit_status = 2;
		free_token_list(data->tokens);
		return (1);
	}
	return (0);
}

static int	execute_main(t_exec_data *data, t_shell *shell)
{
	t_cleanup	cleanup;

	free_token_list(data->tokens);
	setup_execution_signals();
	shell->last_exit_status = execute_ast(data->ast, shell);
	if (shell->last_exit_status == -1)
	{
		cleanup.tokens = data->tokens;
		cleanup.ast = data->ast;
		cleanup.input = data->input;
		cleanup_and_exit_main(shell, &cleanup);
	}
	setup_signal_handlers();
	free_ast(data->ast);
	return (0);
}

void	run_main_loop(t_shell *shell, int input_status)
{
	t_exec_data	data;

	while (1)
	{
		handle_sigint_in_main(shell);
		data.input = readline("minishell$");
		input_status = handle_empty_input(data.input);
		if (input_status == 1)
			break ;
		if (input_status == 2)
			continue ;
		add_history(data.input);
		if (process_tokens(data.input, shell, &data))
		{
			free(data.input);
			continue ;
		}
		if (process_ast(&data, shell))
		{
			free(data.input);
			continue ;
		}
		execute_main(&data, shell);
		free(data.input);
	}
}
