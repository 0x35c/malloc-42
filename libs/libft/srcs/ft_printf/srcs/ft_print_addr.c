/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_addr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulayus <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 14:46:13 by ulayus            #+#    #+#             */
/*   Updated: 2024/03/12 11:23:04 by ulayus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_put_addr(int fd, unsigned long addr, char *base, int reset)
{
	unsigned long	size;
	static int		count_addr;

	if (reset)
		count_addr = 0;
	size = 16;
	if (addr < size)
		count_addr += ft_putchar_v(fd, base[addr % size]);
	else
	{
		ft_put_addr(fd, (addr / size), base, 0);
		ft_put_addr(fd, (addr % size), base, 0);
	}
	return (count_addr);
}

int	ft_print_addr(int fd, unsigned long addr)
{
	int	len;

	len = 0;
	if (!addr)
	{
		len = ft_putstr_v(fd, "(nil)");
		return (len);
	}
	len = ft_putstr_v(fd, "0x");
	len += ft_put_addr(fd, addr, BASE_HEX_LW, 1);
	return (len);
}
