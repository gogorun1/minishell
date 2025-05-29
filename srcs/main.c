/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 13:55:40 by lcao              #+#    #+#             */
/*   Updated: 2025/05/29 15:25:30 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int main (int argc, char **argv, char **envp)
{
	char	*input;
	t_token	*tokens;
	// pid_t	pid;
	t_shell shell;
	ast_node_t	*ast;

	(void)argc;
	(void)argv;
	if (init_shell(&shell, envp) != 0)
		return (1);
	while (1)
	{
		input = readline("minishell$");
		if (!input || strcmp(input, "exit") == 0)
		{
			free(input);
			break;
		}
		// If the input is empty, continue to the next iteration
		if (*input)
			add_history(input);
		printf("Input received: %s\n", input);
		tokens = tokenizer(input, &shell);
		if (!tokens)
		{
			fprintf(stderr, "Error: Tokenization failed\n");
			free(input);
			continue;
		}
		print_tokens(tokens);
		// Parse the tokens into an abstract syntax tree (AST)
		ast = parse(tokens);
		if (!ast)
		{
			fprintf(stderr, "minishell: parse error\n");
			free_token(tokens);
			free(input);
			continue;
		}
		else
		{
			printf("--- 抽象语法树 (AST) 结构 ---\n");
			print_ast(ast, 0); // 从根节点开始打印，初始缩进为 0
			printf("--- AST 打印结束 ---\n");
		}
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
		// if (ast->type != AST_COMMAND)
		// {
		// 	fprintf(stderr, "minishell: only single commands are supported\n");
		// 	free_token(tokens);
		// 	free_ast(ast);
		// 	free(input);
		// 	continue;
		// }
		
		// Check if the command is a builtin
		// if (is_builtin(ast->data.command.args[0]))
		// {
		// 	// If the command is a builtin, run it directly
		// 	shell.last_exit_status = run_builtin(ast->data.command.args, &shell.env_list);
		// 	free_token(tokens);
		// 	free_ast(ast);
		// 	free(input);
		// 	continue;
		// }
		// pid = fork();
		// if (pid == 0)
		// {
		// 	if (execvp(args[0], args) == -1)
		// 		perror("minishell");
		// 	exit(1);
		// }
		// else
		// 	wait(NULL);
	}
	rl_clear_history();
	return (0);
}
