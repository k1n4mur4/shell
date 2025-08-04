/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_input_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 04:30:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:30:00 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exit_value.h"
#include "make_cmd.h"
#include "minishell.h"
#include "redirect_utils.h"
#include "shell_helpers.h"
#include "signal_handler.h"
#include <unistd.h>

static char	*process_buffer_char(char *line, int *line_len, char c)
{
	if (c == '\n')
	{
		if (line)
		{
			line[*line_len] = '\0';
			return (line);
		}
		return (ft_strdup(""));
	}
	if (!line)
		line = ft_calloc(1024, sizeof(char));
	if (line && *line_len < 1023)
		line[(*line_len)++] = c;
	return (line);
}

char	*read_line_with_read(void)
{
	char	buffer[1024];
	char	*line;
	int		bytes_read;
	int		line_len;
	int		i;

	line = NULL;
	line_len = 0;
	bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
	while (bytes_read > 0)
	{
		i = -1;
		while (++i < bytes_read)
		{
			line = process_buffer_char(line, &line_len, buffer[i]);
			if (buffer[i] == '\n')
				return (line);
		}
		bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
	}
	if (line)
		line[line_len] = '\0';
	return (line);
}

char	*helper_append_input_line(char *full_input, char *line)
{
	char	*temp;

	if (full_input)
	{
		temp = ft_strjoin(full_input, line);
		free(full_input);
		return (temp);
	}
	else
	{
		return (ft_strdup(line));
	}
}
