/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 01:54:20 by kinamura          #+#    #+#             */
/*   Updated: 2025/05/06 01:54:57 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*str;

	if (!s)
		return (NULL);
	str = (char *)ft_calloc(n + 1, sizeof(char));
	if (!str)
		return (NULL);
	ft_strlcpy(str, s, n + 1);
	return (str);
}
