/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_filesystem.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:49:35 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "builtin_filesystem_helpers.h"

int	builtin_pwd(t_word_list *args)
{
	char	*cwd;

	(void)args;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_dprintf(STDERR_FILENO, "minishell: pwd: %s\n", strerror(errno));
		return (1);
	}
	ft_dprintf(STDOUT_FILENO, "%s\n", cwd);
	free(cwd);
	return (0);
}

static int	get_new_pwd(char **new_pwd, char *current_pwd)
{
	*new_pwd = getcwd(NULL, 0);
	if (!*new_pwd)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: cd: error retrieving current directory\n");
		free(current_pwd);
		return (0);
	}
	return (1);
}

int	builtin_cd(t_word_list *args)
{
	char	*path;
	char	*current_pwd;
	char	*new_pwd;

	if (!validate_cd_args(args))
		return (1);
	current_pwd = getcwd(NULL, 0);
	path = determine_target_path(args, current_pwd);
	if (!path)
		return (1);
	if (!change_directory(path, current_pwd))
		return (1);
	if (!get_new_pwd(&new_pwd, current_pwd))
		return (1);
	update_pwd_vars(current_pwd, new_pwd);
	free(current_pwd);
	free(new_pwd);
	return (0);
}
