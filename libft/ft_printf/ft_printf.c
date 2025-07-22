/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linliu <linliu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:42:25 by linliu            #+#    #+#             */
/*   Updated: 2025/06/12 14:15:46 by linliu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_type(const char format, va_list *args)
{
	if (format == 's')
		return (write_str(va_arg(*args, const char *)));
	if (format == 'c')
		return (write_char(va_arg(*args, int)));
	if (format == 'i' || format == 'd')
		return (write_int(va_arg(*args, int)));
	if (format == 'u')
		return (write_uint_base(va_arg(*args, unsigned int), "0123456789", 10));
	if (format == 'x')
		return (write_uint_base(va_arg(*args, unsigned int), HEX_LOWER, 16));
	if (format == 'X')
		return (write_uint_base(va_arg(*args, unsigned int), HEX_UPPER, 16));
	if (format == 'p')
		return (write_pointer(va_arg(*args, void *), HEX_LOWER));
	if (format == '%')
		return (write (1, "%", 1));
	return (-1);
}

int	format_loop(const char *format, va_list *args)
{
	int	type;
	int	count;

	count = 0;
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (format == 0)
				return (-1);
			type = check_type(*format, args);
			if (type == -1)
				return (-1);
			count += type;
		}
		else
		{
			if (write(1, format, 1) == -1)
				return (-1);
			count++;
		}
		format++;
	}
	return (count);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		count;

	if (!format || write(1, "", 0) < 0)
		return (-1);
	va_start(args, format);
	count = format_loop(format, &args);
	va_end(args);
	return (count);
}
