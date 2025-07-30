/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:44 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:38:02 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FD_UTILS_H
# define FD_UTILS_H

# include <fcntl.h>
# include <unistd.h>
# include <stdbool.h>

/* File descriptor management */
int		setup_file_redirect(const char *filename, int target_fd, int flags);
int		backup_fd(int fd);
int		restore_fd(int fd, int backup_fd);
void	close_if_valid(int fd);

/* Common redirect operations */
int		setup_input_redirect(const char *filename);
int		setup_output_redirect(const char *filename);
int		setup_append_redirect(const char *filename);

#endif