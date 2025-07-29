/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:37:05 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 01:37:07 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_UTILS_H
# define PATH_UTILS_H

# include "env.h"
# include "libft.h"
# include <sys/stat.h>
# include <unistd.h>

/* Path search functions */
char	*find_command_path(const char *command);
int		is_executable(const char *path);
char	**get_path_dirs(void);

#endif