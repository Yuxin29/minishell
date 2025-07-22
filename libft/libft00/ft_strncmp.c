/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linliu <linliu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:19:18 by linliu            #+#    #+#             */
/*   Updated: 2025/04/28 09:45:37 by linliu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t				i;
	const unsigned char	*n1;
	const unsigned char	*n2;

	i = 0;
	n1 = (const unsigned char *)s1;
	n2 = (const unsigned char *)s2;
	while ((n1[i] || n2[i]) && i < n)
	{
		if (n1[i] != n2[i])
			return (n1[i] - n2[i]);
		i++;
	}
	return (0);
}
