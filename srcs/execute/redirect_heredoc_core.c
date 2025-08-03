/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc_core.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 02:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 02:56:15 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect_utils.h"
#include <signal.h>

static int	_helper_is_delimiter(const char *line, const char *delimiter,
		size_t delim_len)
{
	return (ft_strlen(line) == delim_len && ft_strcmp(line, delimiter) == 0);
}

static char	*_helper_read_line_with_prompt(int original_stdout)
{
	if (isatty(original_stdout))
		dup2(original_stdout, STDOUT_FILENO);
	return (readline("> "));
}

static int	_helper_process_heredoc_line(char **content, char *line,
		const char *delimiter, size_t delim_len)
{
	if (!line)
	{
		free(*content);
		*content = NULL;
		return (0);
	}
	if (_helper_is_delimiter(line, delimiter, delim_len))
	{
		free(line);
		return (0);
	}
	ft_strappend(content, line);
	if (!*content)
		return (0);
	return (1);
}

static int	_helper_setup_heredoc_content_reading(int *original_stdout,
		struct sigaction *old_sa)
{
	*original_stdout = dup(STDOUT_FILENO);
	if (*original_stdout == -1)
		return (0);
	setup_heredoc_signals(old_sa);
	return (1);
}

char	*read_heredoc_content(const char *delimiter)
{
	char				*line;
	char				*content;
	size_t				delim_len;
	struct sigaction	old_sa;
	int					original_stdout;

	content = NULL;
	delim_len = ft_strlen(delimiter);
	if (!_helper_setup_heredoc_content_reading(&original_stdout, &old_sa))
		return (NULL);
	while (1)
	{
		line = _helper_read_line_with_prompt(original_stdout);
		if (!_helper_process_heredoc_line(&content, line, delimiter, delim_len))
			break ;
	}
	sigaction(SIGINT, &old_sa, NULL);
	close(original_stdout);
	return (content);
}
