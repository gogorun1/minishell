/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_clean.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 22:17:54 by wding             #+#    #+#             */
/*   Updated: 2025/06/18 04:31:29 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_and_exit(t_shell *shell, t_token *tokens, t_ast_node *ast,
		char *input)
{
	if (tokens)
		free_token_list(tokens);
	if (ast)
		free_ast(ast);
	if (input)
		free(input);
	rl_clear_history();
	if (shell->env_list)
		free_env(shell->env_list);
	exit(shell->last_exit_status);
}
