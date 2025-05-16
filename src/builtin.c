/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:30:06 by lcao              #+#    #+#             */
/*   Updated: 2025/05/16 17:58:03 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*test if it is builtin*/
#include "minishell.h"

int is_builtin(char *cmd)
{
	if(!cmd)
		return (0);
	if(strcmp(cmd, "cd") == 0)
		return (1);
	if(strcmp(cmd, "exit") == 0)
		return (1);
	if(strcmp(cmd, "pwd") == 0)
		return (1);
	return (0);
}

/*based on the name to run those builtins*/
int	run_builtin(char **args)
{
	if(!args || !args[0])
		return (1);
	if(strcmp(args[0], "cd") == 0)
		return(builtin_cd(args));
	if(strcmp(args[0], "exit") == 0)
		return(builtin_exit());
	if(strcmp(args[0], "pwd") == 0)
		return(builtin_pwd());
	return (0);
}
