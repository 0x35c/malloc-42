/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulayus <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 12:08:12 by ulayus            #+#    #+#             */
/*   Updated: 2024/03/06 13:18:45 by ulayus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_strncmp(char *s1, char *s2, size_t n)
{
	size_t			i;
	unsigned char	*cast1;
	unsigned char	*cast2;

	i = 0;
	cast1 = (unsigned char *) s1;
	cast2 = (unsigned char *) s2;
	if (!n)
		return (0);
	while (i + 1 < n && cast1[i] == cast2[i] && cast1[i])
		++i;
	return (cast1[i] - cast2[i]);
}
