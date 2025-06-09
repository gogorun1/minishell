/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abby <abby@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 13:55:40 by lcao              #+#    #+#             */
/*   Updated: 2025/06/08 14:21:58 by abby             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_status = 0; // For signal handling

// void	free_2d(char **arr)
// {
// 	int	i;

// 	if(!arr)
// 		return ;
// 	i = 0;
// 	while(arr[i])
// 	{
// 		free(arr[i]);
// 		i++;
// 	}
// 	free(arr);
// }

int init_shell(t_shell *shell, char **envp)
{
	shell->env_list = init_env(envp);
	if (!shell->env_list)
	{
		fprintf(stderr, "Error initializing environment variables\n");
		return (1);
	}
	shell->last_exit_status = 0;
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	char	*input;
	t_token	*tokens;
	// pid_t	pid;
	t_shell shell;
	ast_node_t	*ast;

	setup_signal_handlers(); // Set up signal handlers for Ctrl-C and Ctrl-'\'

	(void)argc;
	(void)argv;
	if (init_shell(&shell, envp) != 0)
		return (1);
	while (1)
	{
	 	
		if (g_signal_status == SIGINT)
		{
			shell.last_exit_status = 130; // Set exit status for Ctrl-C
			g_signal_status = 0; // Reset global signal status
			ft_putchar_fd('\r', STDOUT_FILENO);
			continue; // Continue to the next iteration
			// g_signal_status = 0; // Print carriage return to move cursor to the start of the line
		}
		// printf("Debug: about to read input, signal_status: %d\n", g_signal_status);
		input = readline("minishell$");
		// printf("Debug: input read: '%s', signal is %d\n", input ? input : "NULL", g_signal_status);
		if (!input)
		{
			printf("exit\n");
			break; // Exit if EOF is received (Ctrl-D)
		}
		if (input[0] == '\0') // Check if the input is empty
		{
			printf("Empty input, skipping...\n");
			free(input); // Free the empty input
			continue; // Skip to the next iteration
		}
		add_history(input);
		tokens = tokenizer(input, &shell);
		if (!tokens)
		{
			fprintf(stderr, "Error: Tokenization failed\n");
			free(input);
			continue;
		}
		// print_tokens(tokens);
		ast = parse(tokens);
		if (!ast)
		{
			fprintf(stderr, "minishell: parse error\n");
			free_token(tokens);
			free(input);
			continue;
		}
		// else
		// {
		// 	printf("--- 抽象语法树 (AST) 结构 ---\n");
		// 	print_ast(ast, 0); // 从根节点开始打印，初始缩进为 0
		// 	printf("--- AST 打印结束 ---\n");
		// }
		// Execute the command represented by the AST
		shell.last_exit_status = execute_ast(ast, &shell);
		if (shell.last_exit_status == -1)
		{
			ft_fprintf(2, "minishell: execution error\n");
			free_token(tokens);
			free_ast(ast);
			free(input);
			continue;
		}
		// Free the tokens and AST after execution
		free_token(tokens);
		free_ast(ast);
		free(input);
	}
	rl_clear_history();
	// free_env(shell.env_list);
	// clear_history();
	return (shell.last_exit_status);
}
