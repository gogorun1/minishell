/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:56:04 by lcao              #+#    #+#             */
/*   Updated: 2025/05/16 17:03:39 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(void)
{
	/*current working directory*/
	char	*cwd;
	
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("cwd");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
