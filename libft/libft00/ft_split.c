/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linliu <linliu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:39:32 by linliu            #+#    #+#             */
/*   Updated: 2025/04/30 15:49:23 by linliu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	issep(char c, char sep)
{
	if (c == sep)
		return (1);
	return (0);
}

static int	count_words(const char *s, char c)
{
	int	count;
	int	flag;

	flag = 0;
	count = 0;
	while (*s)
	{
		if (!issep(*s, c) && flag == 0)
		{
			flag = 1;
			count++;
		}
		else if (issep(*s, c))
			flag = 0;
		s++;
	}
	return (count);
}

static int	sublen(char const *s, char c)
{
	int	len;

	len = 0;
	while (!issep(*s, c) && *s)
	{
		len++;
		s++;
	}
	return (len);
}

static void	*arr_free(char **arr, int i)
{
	while (i >= 0)
	{
		free (arr[i]);
		i--;
	}
	free(arr);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	int		a;
	char	**arr;

	if (!s)
		return (NULL);
	a = 0;
	arr = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (arr == NULL)
		return (NULL);
	while (*s)
	{
		if (!issep(*s, c))
		{
			arr[a] = ft_substr(s, 0, sublen(s, c));
			if (!arr[a])
				return (arr_free(arr, a));
			s += sublen(s, c);
			a++;
		}
		else
			s++;
	}
	arr[a] = NULL;
	return (arr);
}
