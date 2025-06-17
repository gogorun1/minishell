/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:19:41 by lcao              #+#    #+#             */
/*   Updated: 2025/06/17 23:00:33 by wding            ###   ########.fr       */
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
static void	free_redirections(t_redir *redir)
{
	t_redir	*next;

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
static void	free_command_node(t_ast_node *node)
{
	free_command_args(node->u_data.command.args);
	free_redirections(node->u_data.command.redirs);
}

/* Free pipe node data */
static void	free_pipe_node(t_ast_node *node)
{
	free_ast(node->u_data.s_binary.left);
	free_ast(node->u_data.s_binary.right);
}

/* Free AST node recursively */
void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == AST_COMMAND)
		free_command_node(node);
	else if (node->type == AST_PIPE)
		free_pipe_node(node);
	free(node);
}
