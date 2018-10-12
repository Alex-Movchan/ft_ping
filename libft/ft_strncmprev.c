/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmprev.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amovchan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/23 13:26:26 by amovchan          #+#    #+#             */
/*   Updated: 2016/12/07 17:38:10 by amovchan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strncmprev(const char *s1, const char *s2, size_t n)
{
	int		i;
	int 	j;

	i = ft_strlen(s1);
	j = ft_strlen(s2);
	while(--i >= 0 && --j >= 0 && n--)
	{
		if (s1[i] != s2[j])
			return ((unsigned char)s1[i] - (unsigned char)s2[j]);
	}
	return ((unsigned char)s1[++i] - (unsigned char)s2[++j]);
}
