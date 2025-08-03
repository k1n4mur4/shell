/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_sorting.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 03:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_SORTING_H
# define ENV_SORTING_H

# include "env.h"

// env_sorting.c
int		compare_env_keys(t_env *a, t_env *b);
t_env	*sort_env_list(t_env *env_list);

#endif