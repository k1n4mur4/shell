/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_value.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:36:52 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 01:36:54 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_VALUE_H
# define EXIT_VALUE_H

# include "define.h"

int	exit_value(int exit_value, enum e_type type);
int	shell_exit_status(int exit_code, enum e_type type);

#endif