/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 13:55:40 by lcao              #+#    #+#             */
/*   Updated: 2025/05/16 18:11:22 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main (void)
{
	char	*input;
	char	**args;
	pid_t	pid;

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
		if(strcmp(input, "exit") == 0)
		{
			free(input);
			builtin_exit();
		}
		args = split_input(input);
		
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

