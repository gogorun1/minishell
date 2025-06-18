/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 03:28:25 by wding             #+#    #+#             */
/*   Updated: 2025/06/18 05:06:55 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	event(void)
{
	return (EXIT_SUCCESS);
}

static void	handle_sigint_in_main(t_shell *shell)
{
	if (g_signal_status == SIGINT)
	{
		shell->last_exit_status = 130;
		g_signal_status = 0;
	}
}
// void	cleanup_and_exit(t_shell *shell, t_token *tokens, t_ast_node *ast,
// 		char *input)
// {
// 	if (tokens)
// 		free_token_list(tokens);
// 	if (ast)
// 		free_ast(ast);
// 	if (input)
// 		free(input);
// 	rl_clear_history();
// 	if (shell->env_list)
// 		free_env(shell->env_list);
// 	clear_history();
// 	exit(shell->last_exit_status);
// }

static void	set_signal(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	if (!isatty(STDOUT_FILENO))
	{
		printf("Redirection mode forbidden\n");
		exit(1);
	}
	if (!isatty(STDIN_FILENO))
	{
		printf("Non interactive mode forbidden\n");
		exit(1);
	}
	setup_signal_handlers();
	rl_event_hook = event;
}

static int	init_shell(t_shell *shell, char **envp, int argc, char **argv)
{
	set_signal(argc, argv);
	shell->env_list = init_env(envp);
	if (!shell->env_list)
	{
		ft_fprintf(2, "Error initializing environment variables\n");
		return (1);
	}
	shell->last_exit_status = 0;
	return (0);
}

static t_ast_node *token_parse_in_main(char *in, t_token *tokens, t_shell *shell)
{
	t_ast_node	*ast;
	
	(*shell).last_exit_status = 0;
	tokens = tokenizer(in, shell);
	if (!tokens)
	{
		(*shell).last_exit_status = 1;
		free(in);
		return (NULL);
	}
	if (!in)
		free(in);
	ast = parse(tokens);
	if (!ast)
	{
		if (g_signal_status == 130)
		{
			(*shell).last_exit_status = 130;
			g_signal_status = 0;
		}
		else
			(*shell).last_exit_status = 2;
	}
	free_token_list(tokens);
	return (ast);
}

static void	run_main_loop(char *in, t_token *tokens, t_shell *shell, t_ast_node *ast)
{
	while (1)
	{
		handle_sigint_in_main(shell);
		in = readline("minishell$");
		if (!in)
		{
			printf("exit\n");
			cleanup_and_exit(shell, tokens, ast, in);
			break ;
		}
		else if (in[0] == '\0')
		{
			free(in);
			continue ;
		}
		else
		{
			add_history(in);
			ast = token_parse_in_main(in, tokens, shell);
		}
		setup_execution_signals();
		(*shell).last_exit_status = execute_ast(ast, shell);
		if ((*shell).last_exit_status == -1)
			cleanup_and_exit(shell, tokens, ast, in);
		setup_signal_handlers();
	}
}

int	main(int argc, char **argv, char **envp)
{
	char		*in;
	t_token		*tokens;
	t_shell		shell;
	t_ast_node	*ast;

	tokens = NULL;
	ast = NULL;
	in = NULL;
	if (init_shell(&shell, envp, argc, argv) != 0)
		return (1);
	run_main_loop(in, tokens, &shell, ast);
	rl_clear_history();
	if(shell.env_list)
		free_env(shell.env_list);
	return (shell.last_exit_status);
}
