/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulayus <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 22:47:50 by ulayus            #+#    #+#             */
/*   Updated: 2024/03/12 11:40:15 by ulayus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdarg.h>
#include <unistd.h>
#define BASE_10 "0123456789"
#define BASE_HEX_UP "0123456789ABCDEF"
#define BASE_HEX_LW "0123456789abcdef"

int	ft_printf(const char *format, ...);
int	ft_dprintf(int fd, const char *format, ...);
int	ft_putchar_v(int fd, char c);
int	ft_putstr_v(int fd, char *s);
int	ft_putnbase(int fd, long long nb, char *base, int reset);
int	ft_print_addr(int fd, unsigned long addr);
