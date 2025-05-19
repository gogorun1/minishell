/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 13:55:40 by lcao              #+#    #+#             */
/*   Updated: 2025/05/19 16:00:05 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_2d(char **arr)
{
	int	i;

	if(!arr)
		return ;
	i = 0;
	while(arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int main (int argc, char **argv, char **envp)
{
	char	*input;
	char	**args;
	pid_t	pid;

	(void)argc;
	(void)argv;

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
		args = split_input(input);
		
		if (!args || !args[0])
		{
			free(input);
			continue;
		}
		if (is_builtin(args[0]))
		{
			run_builtin(args, envp);
			free_2d(args);
			free(input);
			continue;
		}
		pid = fork();
		if (pid == 0)
		{
			if (execvp(args[0], args) == -1)
				perror("minishell");
			exit(1);
		}
		else
			wait(NULL);
		free(input);
	}
	return (0);
}
