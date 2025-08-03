/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 02:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:53:21 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "execute_cmd.h"
#include "redirect_utils.h"

static int	_helper_handle_no_words(t_simple_command *simple)
{
	t_redirect_backup	backup;

	init_redirect_backup(&backup);
	if (simple->redirects)
	{
		if (setup_redirections(simple->redirects, &backup) == -1)
		{
			restore_redirections(&backup);
			return (1);
		}
	}
	restore_redirections(&backup);
	return (0);
}

static int	_helper_setup_redirects(t_simple_command *simple,
		t_redirect_backup *backup)
{
	if (simple->redirects)
	{
		if (setup_redirections(simple->redirects, backup) == -1)
		{
			restore_redirections(backup);
			return (1);
		}
	}
	return (0);
}

static int	_helper_execute_command_word(t_simple_command *simple)
{
	if (is_builtin_command(simple->words->word->word))
		return (execute_builtin(simple->words->word->word,
				simple->words->next));
	else
		return (execute_external_command_wrapper(simple->words->word->word,
				simple->words->next));
}

static int	_helper_validate_simple_command(t_simple_command *simple)
{
	if (!simple)
		return (0);
	if (!simple->words)
		return (_helper_handle_no_words(simple));
	if (!simple->words->word || !simple->words->word->word)
		return (0);
	return (-1);
}

int	execute_simple_command(t_simple_command *simple)
{
	t_redirect_backup	backup;
	int					exit_code;
	int					validation_result;

	validation_result = _helper_validate_simple_command(simple);
	if (validation_result >= 0)
		return (validation_result);
	init_redirect_backup(&backup);
	if (_helper_setup_redirects(simple, &backup) == 1)
		return (1);
	exit_code = _helper_execute_command_word(simple);
	if (simple->redirects)
		restore_redirections(&backup);
	return (exit_code);
}
