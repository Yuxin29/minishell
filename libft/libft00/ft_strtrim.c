/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linliu <linliu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:21:04 by linliu            #+#    #+#             */
/*   Updated: 2025/04/28 10:11:48 by linliu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_inset(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	begin;
	size_t	end;
	char	*str;
	size_t	i;

	if (!s1 || !set)
		return (NULL);
	begin = 0;
	while (s1[begin] && is_inset(s1[begin], set))
		begin++;
	end = ft_strlen(s1);
	while (end > begin && is_inset(s1[end - 1], set))
		end--;
	str = malloc(sizeof(char) * (end - begin + 1));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < end - begin)
	{
		str[i] = s1[begin + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
