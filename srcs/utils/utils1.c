/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:19:41 by lcao              #+#    #+#             */
/*   Updated: 2025/06/17 21:19:52 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Free command arguments array */
static void	free_command_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

/* Free redirection list */
static void	free_redirections(redir_t *redir)
{
	redir_t	*next;

	while (redir)
	{
		next = redir->next;
		if (redir->file)
			free(redir->file);
		if (redir->heredoc_content)
			free(redir->heredoc_content);
		free(redir);
		redir = next;
	}
}

/* Free command node data */
static void	free_command_node(ast_node_t *node)
{
	free_command_args(node->data.command.args);
	free_redirections(node->data.command.redirs);
}

/* Free pipe node data */
static void	free_pipe_node(ast_node_t *node)
{
	free_ast(node->data.binary.left);
	free_ast(node->data.binary.right);
}

/* Free AST node recursively */
void	free_ast(ast_node_t *node)
{
	if (!node)
		return ;
	if (node->type == AST_COMMAND)
		free_command_node(node);
	else if (node->type == AST_PIPE)
		free_pipe_node(node);
	free(node);
}
