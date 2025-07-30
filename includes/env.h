/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:36:36 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 01:36:38 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "define.h"
# include "ft_printf.h"
# include "libft.h"
# include <unistd.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

t_env				*alloc_env(void);
t_env				*make_env_list(char *key, char *value);
t_env				*initialize_env(char *envp);
void				initialize_enviroment(char **envp);

void				dispose_env(t_env *env_list);
t_env				*search_env(t_env *env, char *key);
void				add_env_value(t_env *env, t_env *lst);
void				add_env_list(t_env *env, t_env *env_list);
void				show_env_list(t_env *env_list);
t_env				*env(t_env *env_list, enum e_type type);

int					compare_env_keys(t_env *a, t_env *b);
t_env				*sort_env_list(t_env *env_list);
t_env				*copy_env_list(t_env *env_list);
void				free_env_list(t_env *env_list);

#endif