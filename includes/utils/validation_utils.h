/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_utils.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:44 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:38:02 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATION_UTILS_H
# define VALIDATION_UTILS_H

# include "libft.h"
# include <stdbool.h>
# include <limits.h>

/* Input validation functions */
bool	is_valid_identifier(const char *str);
bool	is_valid_number(const char *str, long *result);
bool	is_numeric_string(const char *str);

#endif