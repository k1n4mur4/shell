/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_env_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 04:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:06:36 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

t_env	*alloc_env(void)
{
	t_env	*temp;

	temp = (t_env *)ft_calloc(1, sizeof(t_env));
	if (!temp)
		return (NULL);
	temp->key = NULL;
	temp->value = NULL;
	temp->next = NULL;
	return (temp);
}
