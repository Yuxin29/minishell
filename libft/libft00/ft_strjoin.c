/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linliu <linliu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:17:02 by linliu            #+#    #+#             */
/*   Updated: 2025/04/25 12:31:27 by linliu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	total_len;
	char	*str;
	char	*ptr;

	if (!s1 || !s2)
		return (NULL);
	total_len = ft_strlen(s1) + ft_strlen(s2);
	str = malloc(sizeof(char) * (total_len + 1));
	if (str == NULL)
		return (NULL);
	ptr = str;
	while (*s1)
		*ptr++ = *s1++;
	while (*s2)
		*ptr++ = *s2++;
	*ptr = '\0';
	return (str);
}
