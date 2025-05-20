/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-18 16:57:20 by wding             #+#    #+#             */
/*   Updated: 2025-05-18 16:57:20 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token: %s, Type: %d\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
}

int main()
{
	int status;
	char *line = NULL;
	t_token *tokens = NULL;
	// t_cmd *cmds = NULL;

	while (1)
	{
		line = readline("minishell> ");
		if (line == NULL)
			break;
		if (strcmp(line, "exit") == 0)
		{
			free(line);
			break;
		}
		// Tokenize the input line
		tokens = tokenizer(line);
		if (tokens == NULL)
		{
			fprintf(stderr, "Error: Tokenization failed\n");
			free(line);
			continue;
		}
		print_tokens(tokens);
		
		// Parse the tokens into commands

		// Execute the commands
		// Free the command list

		// Free the token list
		free_token_list(tokens);

		// Add the line to history
		add_history(line);
		free(line);
	}
	wait(&status);
	return EXIT_SUCCESS;
}

