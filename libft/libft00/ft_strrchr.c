/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linliu <linliu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:59:40 by linliu            #+#    #+#             */
/*   Updated: 2025/04/24 17:58:55 by linliu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	src_len;

	if (!s)
		return (NULL);
	src_len = ft_strlen(s);
	while (src_len >= 0)
	{
		if (s[src_len] == (char)c)
			return ((char *)(&s[src_len]));
		src_len--;
	}
	return (NULL);
}
