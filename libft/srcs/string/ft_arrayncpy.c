/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrayncpy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 01:52:36 by kinamura          #+#    #+#             */
/*   Updated: 2025/05/06 01:52:38 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_arrayncpy(char **str, int n)
{
	int		index;
	char	**copy;
	char	**p;

	if (!str)
		return (NULL);
	p = str;
	copy = (char **)ft_calloc(n + 1, sizeof(char *));
	if (!copy)
		return (NULL);
	index = 0;
	while (index < n && *p)
	{
		copy[index] = ft_strdup(*p);
		if (!copy[index])
		{
			ft_free_array2((void **)copy);
			return (NULL);
		}
		p++;
		index++;
	}
	copy[index] = NULL;
	return (copy);
}
