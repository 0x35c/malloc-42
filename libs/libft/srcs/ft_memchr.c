/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulayus <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 15:26:14 by ulayus            #+#    #+#             */
/*   Updated: 2024/03/06 13:19:44 by ulayus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const char	*str;

	if (!n)
		return (NULL);
	while (c > 255)
		c -= 256;
	str = s;
	if (c == '\0')
	{
		s += ft_strlen(str);
		return ((void *)s);
	}
	while (n)
	{
		if (*str == c)
			return ((void *)s);
		--n;
		++str;
		++s;
	}
	return (NULL);
}
