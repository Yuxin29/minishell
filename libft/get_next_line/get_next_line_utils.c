/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linliu <linliu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:07:37 by linliu            #+#    #+#             */
/*   Updated: 2025/06/12 15:31:47 by linliu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr_gnl(char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == (char)c)
			return (s);
		s++;
	}
	if (c == '\0')
		return (s);
	return (NULL);
}

size_t	ft_strlen_gnl(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*str;
	size_t	i;
	size_t	j;

	if (!s2)
		return (NULL);
	if (!s1)
	{
		s1 = malloc(1);
		if (!s1)
			return (NULL);
		s1[0] = '\0';
	}
	str = malloc(sizeof(char) * (ft_strlen_gnl(s1) + ft_strlen_gnl(s2) + 1));
	if (!str)
		return (free (s1), NULL);
	i = -1;
	while (s1[++i])
		str[i] = s1[i];
	j = 0;
	while (s2[j])
		str[i++] = s2[j++];
	str[i] = '\0';
	free (s1);
	return (str);
}

char	*ft_strndup_gnl(const char *s, size_t n)
{
	char	*dst;
	size_t	i;

	if (!s)
		return (NULL);
	dst = malloc(sizeof(char) * (n + 1));
	if (!dst)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dst[i] = s[i];
		i++;
	}
	dst[n] = '\0';
	return (dst);
}
