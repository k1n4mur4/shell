#ifndef BUILTIN_H
#define BUILTIN_H

#include "command.h"
#include "env.h"
#include "exit_value.h"
#include "libft.h"
#include "ft_printf.h"
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

/* Builtin command function declarations */
int	builtin_echo(t_word_list *args);
int	builtin_cd(t_word_list *args);
int	builtin_pwd(t_word_list *args);
int	builtin_export(t_word_list *args);
int	builtin_unset(t_word_list *args);
int	builtin_env(t_word_list *args);
int	builtin_exit(t_word_list *args);

/* Builtin helper functions */
int	is_builtin_command(const char *command);
int	execute_builtin(const char *command, t_word_list *args);

#endif