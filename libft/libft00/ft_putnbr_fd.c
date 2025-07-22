/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linliu <linliu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:10:26 by linliu            #+#    #+#             */
/*   Updated: 2025/04/25 14:14:34 by linliu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	number;

	number = (long)n;
	if (number < 0)
	{
		ft_putchar_fd('-', fd);
		number = -number;
	}
	if (number > 9)
	{
		ft_putnbr_fd(number / 10, fd);
	}
	ft_putchar_fd(number % 10 + '0', fd);
}
