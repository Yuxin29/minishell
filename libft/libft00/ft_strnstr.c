/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linliu <linliu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:37:03 by linliu            #+#    #+#             */
/*   Updated: 2025/04/18 17:05:47 by linliu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	lit_len;

	i = 0;
	lit_len = ft_strlen(little);
	if (*little == '\0')
		return ((char *)big);
	while (big[i] && i + lit_len <= len)
	{
		if (big[i] == *little)
		{
			if (ft_strncmp(&big[i], little, lit_len) == 0)
			{
				return ((char *)&big[i]);
			}
		}
		i++;
	}
	return (NULL);
}
