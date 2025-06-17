#include "minishell.h"

int	init_shell(t_shell *shell, char **envp)
{
	shell->env_list = init_env(envp);
	if (!shell->env_list)
	{
		ft_fprintf(2, "Error initializing environment variables\n");
		return (1);
	}
	shell->last_exit_status = 0;
	return (0);
}

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
int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_token		*tokens;
	t_shell		shell;
	t_ast_node	*ast;

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
	if (init_shell(&shell, envp) != 0)
		return (1);
	rl_event_hook = event;
	while (1)
	{
		handle_sigint_in_main(&shell);
		input = readline("minishell$");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		add_history(input);
		tokens = tokenizer(input, &shell);
		if (!tokens)
		{
			shell.last_exit_status = 1;
			free(input);
			continue ;
		}
		ast = parse(tokens);
		if (!ast)
		{
			if (g_signal_status == 130)
			{
				shell.last_exit_status = 130;
				g_signal_status = 0;
			}
			else
				shell.last_exit_status = 2;
			free_token_list(tokens);
			free(input);
			continue ;
		}
		free_token_list(tokens);
		setup_execution_signals();
		shell.last_exit_status = execute_ast(ast, &shell);
		if (shell.last_exit_status == -1)
			cleanup_and_exit(&shell, tokens, ast, input);
		setup_signal_handlers();
		free_ast(ast);
		free(input);
	}
	rl_clear_history();
	free_env(shell.env_list);
	clear_history();
	return (shell.last_exit_status);
}
