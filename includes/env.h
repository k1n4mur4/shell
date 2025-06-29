#ifndef ENV_H
#define ENV_H

#include <unistd.h>
#include "type.h"

#include "libft.h"
#include "ft_printf.h"

typedef struct	s_env
{
	char	*key;
	char	*value;
	struct s_env	*next;
}				t_env;

// initialize_enviroment.c
t_env	*alloc_env(void);
t_env	*make_env_list(char *key, char *value);
t_env	*initialize_env(char *envp);
void	initialize_enviroment(char **envp);

// env.c
void	dispose_env(t_env *env_list);
t_env	*search_env(t_env *env, char *key);
void	add_env_value(t_env *env, t_env *lst);
void	add_env_list(t_env *env, t_env *env_list);
void	show_env_list(t_env *env_list);
t_env	*env(t_env *env_list, enum e_type type);

#endif