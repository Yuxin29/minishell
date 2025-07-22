/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linliu <linliu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:47:23 by linliu            #+#    #+#             */
/*   Updated: 2025/04/25 12:35:56 by linliu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*ptr;
	t_list	*tmp;

	if (lst == NULL || del == NULL)
		return ;
	ptr = *lst;
	while (ptr)
	{
		tmp = ptr -> next;
		del (ptr -> content);
		free (ptr);
		ptr = tmp;
	}
	*lst = NULL;
}
