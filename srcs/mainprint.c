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
		ast_node_t *ast = parse(tokens);
		if (ast == NULL)
		{
			fprintf(stderr, "Error: Parsing failed\n");
			free(line);
			free_token_list(tokens);
			continue;
		}
		print_ast_detailed(ast, 0, "AST: ");
		// Execute the commands
		// Free the ast
		free_ast(ast);

		// Free the token list
		free_token_list(tokens);

		// Add the line to history
		add_history(line);
		free(line);
	}
	wait(&status);
	return EXIT_SUCCESS;
}

