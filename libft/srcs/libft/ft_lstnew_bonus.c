/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 01:27:42 by wding             #+#    #+#             */
/*   Updated: 2024/11/27 02:15:27 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Allocates (with malloc(3)) and returns a new node.
The member variable ’content’ is initialized with
the value of the parameter ’content’. The variable
’next’ is initialized to NULL.
*/

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}

/*
#include <stdio.h> 

void	cmpcontent(t_list *node, void *content)
{
	if (node->content == content)
		printf("Test 1 passed: content matches\n");
    else
        printf("Test 1 failed: content mismatch\n");

    if (node->next == NULL)
        printf("Test 1 passed: next is NULL\n");
    else
        printf("Test 1 failed: next is not NULL\n");
}

int	main(void)
{
    char *content = "Hello, libft!";
    t_list *node = ft_lstnew(content);

    if (!node)
    {
        printf("ft_lstnew failed: returned NULL\n");
        return (1);
    }
	cmpcontent(node, content);
    free(node);

    int *int_content = malloc(sizeof(int));
    if (!int_content)
    {
        printf("Memory allocation failed for int_content\n");
        return (1);
    }
    *int_content = 42;
    node = ft_lstnew(int_content);
	cmpcontent(node, int_content);
    free(int_content);
    free(node);
    return (0);
}*/
