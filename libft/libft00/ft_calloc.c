/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linliu <linliu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 21:28:32 by linliu            #+#    #+#             */
/*   Updated: 2025/04/25 09:52:02 by Lin Liu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	i;
	void	*ptr;

	if (count != 0 && size > SIZE_MAX / count)
		return (NULL);
	i = count * size;
	ptr = malloc(i);
	if (ptr == NULL)
		return (NULL);
	ft_bzero(ptr, i);
	return (ptr);
}
