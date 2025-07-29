/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_random.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 19:19:53 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/20 14:26:24 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_random(void)
{
	static long	temp;
	int			stack;
	long		rand;

	temp = 0;
	rand = 0;
	temp += (long)&stack;
	temp ^= temp << 5;
	temp ^= temp >> 13;
	temp ^= temp << 17;
	rand = temp;
	return (rand);
}
