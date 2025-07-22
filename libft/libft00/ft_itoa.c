/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linliu <linliu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:49:47 by linliu            #+#    #+#             */
/*   Updated: 2025/04/25 12:33:59 by linliu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_len(int n)
{
	int		len;
	long	number;

	len = 0;
	if (n < 0)
		len++;
	number = n;
	while (number)
	{
		number = number / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int		len;
	long	number;
	char	*arr;

	if (n == 0)
		return (ft_strdup("0"));
	len = count_len(n);
	arr = malloc(sizeof(char) * (len + 1));
	if (!arr)
		return (NULL);
	arr[len] = '\0';
	number = n;
	if (number < 0)
	{
		arr[0] = '-';
		number = -number;
	}
	while (number > 0)
	{
		arr[len - 1] = number % 10 + '0';
		number = number / 10;
		len--;
	}
	return (arr);
}
