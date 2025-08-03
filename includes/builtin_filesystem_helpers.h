/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_filesystem_helpers.h                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 03:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_FILESYSTEM_HELPERS_H
# define BUILTIN_FILESYSTEM_HELPERS_H

# include "command.h"
# include "env.h"

// builtin_pwd_utils.c
void	update_existing_env(t_env *node, char *value);
void	create_new_env(const char *key, char *value);
void	update_oldpwd_var(char *old_pwd);
void	update_pwd_var(char *new_pwd);
void	update_pwd_vars(char *old_pwd, char *new_pwd);

// builtin_cd_utils.c
char	*get_home_path(char *current_pwd);
char	*get_oldpwd_path(char *current_pwd);
char	*determine_target_path(t_word_list *args, char *current_pwd);
int		validate_cd_args(t_word_list *args);
int		change_directory(const char *path, char *current_pwd);

#endif