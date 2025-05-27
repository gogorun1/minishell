/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 13:55:40 by lcao              #+#    #+#             */
/*   Updated: 2025/05/24 15:30:37 by lcao             ###   ########.fr       */
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

int main (int argc, char **argv, char **envp)
{
	char	*input;
	t_token	*tokens;
	// pid_t	pid;
	t_env	*env_list;
	// ast_node_t	*ast;

	(void)argc;
	(void)argv;
	env_list = init_env(envp);
	

	while (1)
	{
		input = readline("minishell$");
		
		if(!input)
		{
			printf("bye!\n");
			break;
		}
		if (*input)
			add_history(input);
		tokens = tokenizer(input);
		if (!tokens)
		{
			free(input);
			continue;
		}
		// ast = parse(tokens);
		// if (!ast)
		// {
		// 	fprintf(stderr, "minishell: parse error\n");
		// 	free_2d(tokens);
		// 	free(input);
		// 	continue;
		// }
		if (is_builtin(tokens[0].value))
		{
			run_builtin(tokens[0].value, &env_list);
			free_token(tokens);
			free(input);
			continue;
		}
		// pid = fork();
		// if (pid == 0)
		// {
		// 	if (execvp(args[0], args) == -1)
		// 		perror("minishell");
		// 	exit(1);
		// }
		// else
		// 	wait(NULL);
		free(input);
	}
	rl_clear_history();
	return (0);
}
