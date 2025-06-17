/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:27:14 by wding             #+#    #+#             */
/*   Updated: 2025/06/17 22:16:11 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static void	free_resources(t_cleanup *cleanup)
{
	if (cleanup->tokens)
		free_token_list(cleanup->tokens);
	if (cleanup->ast)
		free_ast(cleanup->ast);
	if (cleanup->input)
		free(cleanup->input);
}

void	cleanup_and_exit_main(t_shell *shell, t_cleanup *cleanup)
{
	free_resources(cleanup);
	rl_clear_history();
	if (shell->env_list)
		free_env(shell->env_list);
	clear_history();
	exit(shell->last_exit_status);
}
