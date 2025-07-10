#ifndef PROCESS_UTILS_H
#define PROCESS_UTILS_H

#include "command.h"
#include "libft.h"
#include "ft_printf.h"
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

/* Process execution functions */
int		execute_external_command(const char *command_path, const char *command_name, t_word_list *args);
char	**create_argv_array(const char *command_path, const char *command_name, t_word_list *args);
char	**create_envp_array(void);
int		wait_for_child_process(pid_t pid);

/* Helper functions */
void	free_argv_array(char **argv);
void	free_envp_array(char **envp);
int		count_word_list(t_word_list *list);

#endif