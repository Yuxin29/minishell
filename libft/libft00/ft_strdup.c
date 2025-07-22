/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linliu <linliu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 21:27:36 by linliu            #+#    #+#             */
/*   Updated: 2025/04/25 09:47:56 by Lin Liu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*dst;
	size_t	len;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	dst = malloc(sizeof(char) * (len + 1));
	if (dst == NULL)
		return (NULL);
	dst = ft_memcpy(dst, s1, len);
	dst[len] = '\0';
	return (dst);
}
