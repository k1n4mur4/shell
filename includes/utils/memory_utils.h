/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:44 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:38:02 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_UTILS_H
# define MEMORY_UTILS_H

# include "libft.h"
# include <stdlib.h>

/* Generic memory management functions */
void	free_string_array(char **array);
void	free_linked_list(void *head, void (*free_node)(void *));

/* Specific memory management for common structures */
void	free_env_node(void *node);
void	cleanup_env_list(void *env_list);

#endif