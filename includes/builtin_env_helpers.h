/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_helpers.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 03:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:00:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_ENV_HELPERS_H
# define BUILTIN_ENV_HELPERS_H

# include "command.h"
# include "env.h"

// builtin_export_utils.c
void	free_key_value(char *key, char *value);
void	add_env_var(char *key, char *value);
void	process_with_equal(char *word);
void	process_without_equal(char *word);
int		validate_identifier(char *word);

// builtin_unset_utils.c
void	remove_env_node(t_env *to_remove, t_env *prev);
int		find_and_remove_env(const char *key);

#endif