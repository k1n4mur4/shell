/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_append.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 01:52:57 by kinamura          #+#    #+#             */
/*   Updated: 2025/05/06 01:54:01 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_append(char ***basic_str, char *str)
{
	int		count;
	int		index;
	char	**p;

	count = 0;
	if (basic_str == NULL || str == NULL)
		return (NULL);
	if (*basic_str != NULL)
	{
		while ((*basic_str)[count])
			count++;
	}
	p = ft_arrayncpy(*basic_str, count + 1);
	if (!p)
		return (NULL);
	p[count] = ft_strdup(str);
	if (!p[count])
	{
		ft_free_array2((void **)p);
		return (NULL);
	}
	p[count + 1] = NULL;
	ft_free_array2((void **)*basic_str);
	*basic_str = p;
	return (p);
}
