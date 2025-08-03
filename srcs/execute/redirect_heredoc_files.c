/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc_files.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 02:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:51:07 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect_utils.h"

int	create_heredoc_temp_file(char **template, t_heredoc_context *context)
{
	int	fd;

	*template = ft_strdup("/tmp/.minishell_heredoc_XXXXXX");
	if (context)
		context->tempfile = *template;
	fd = ft_mkstemp(*template);
	if (fd < 0)
	{
		print_redirect_error(*template, strerror(errno));
		if (*template)
			free(*template);
		return (-1);
	}
	return (fd);
}

int	write_heredoc_content_to_file(int fd, const char *delimiter)
{
	char	*content;
	int		result;

	content = read_heredoc_content(delimiter);
	if (content)
		result = ft_fputs(content, fd);
	else
		result = 0;
	return (result);
}

int	handle_heredoc_redirect(const char *delimiter)
{
	char				*template;
	int					fd;
	int					result;
	t_heredoc_context	context;

	if (!delimiter)
		return (-1);
	context.tempfile = NULL;
	context.fd = -1;
	context.delimiter = (char *)delimiter;
	fd = create_heredoc_temp_file(&template, &context);
	if (fd < 0)
		return (-1);
	context.fd = fd;
	result = write_heredoc_content_to_file(fd, delimiter);
	close(fd);
	if (result >= 0)
		result = handle_input_redirect(template);
	cleanup_temp_file(template);
	return (result);
}
