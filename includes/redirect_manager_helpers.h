/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_manager_helpers.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 04:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_MANAGER_HELPERS_H
# define REDIRECT_MANAGER_HELPERS_H

# include "redirect_utils.h"

void		init_redirect_backup(t_redirect_backup *backup);
t_redirect	*find_last_redirect(t_redirect *redirects, int types);

#endif