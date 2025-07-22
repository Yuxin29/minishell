/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linliu <linliu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:26:53 by linliu            #+#    #+#             */
/*   Updated: 2025/06/12 14:16:05 by linliu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	write_str(const char *s)
{
	int	len;

	if (!s)
		return (write (1, "(null)", 6));
	len = 0;
	while (s[len])
		len++;
	return (write (1, s, len));
}
