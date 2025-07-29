/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 00:04:41 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/23 00:05:11 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect_utils.h"

void	cleanup_temp_file(char *template)
{
	if (template)
	{
		free(template);
		unlink(template);
	}
}

char	*ft_strappend(char **s1, char *s2)
{
	char	*str;
	size_t	s1_len;
	size_t	s2_len;

	s1_len = ft_strlen(*s1);
	s2_len = ft_strlen(s2);
	str = (char *)ft_calloc(sizeof(char), (s1_len + s2_len + 1));
	if (!str)
	{
		if (*s1)
			free(*s1);
		if (s2)
			free(s2);
		return (NULL);
	}
	if (*s1)
		ft_strlcpy(str, *s1, s1_len + 1);
	if (s2)
		ft_strlcat(str, s2, s1_len + s2_len + 1);
	if (*s1)
		free(*s1);
	if (s2)
		free(s2);
	*s1 = str;
	return (*s1);
}

static char	*read_heredoc_content(const char *delimiter)
{
	char	*line;
	char	*content;
	size_t	delim_len;

	content = NULL;
	delim_len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			free(content);
			return (NULL);
		}
		if (ft_strlen(line) == delim_len && ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_strappend(&content, line);
		if (!content)
			return (NULL);
	}
	return (content);
}

int	handle_heredoc_redirect(const char *delimiter)
{
	char	*template;
	char	*content;
	int		fd;
	int		result;

	if (!delimiter)
		return (-1);
	template = ft_strdup("/tmp/.minishell_heredoc_XXXXXX");
	fd = ft_mkstemp(template);
	if (fd < 0)
	{
		print_redirect_error(template, strerror(errno));
		if (template)
			free(template);
		return (-1);
	}
	content = read_heredoc_content(delimiter);
	result = ft_fputs(content, fd);
	close(fd);
	if (result >= 0)
		result = handle_input_redirect(template);
	cleanup_temp_file(template);
	return (result);
}
