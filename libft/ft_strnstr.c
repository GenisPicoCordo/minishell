/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:06:22 by gpico-co          #+#    #+#             */
/*   Updated: 2024/09/26 13:09:40 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	j = 0;
	i = 0;
	if (little[0] == '\0')
	{
		return ((char *) big);
	}
	while (big[i] != '\0' && i < len)
	{
		while (big[i + j] == little[j] && i + j < len)
		{
			if (little[j + 1] == '\0')
			{
				return ((char *) &big[i]);
			}
			j++;
		}
		j = 0;
		i++;
	}
	return (0);
}
