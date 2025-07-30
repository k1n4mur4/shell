/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 00:04:41 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:46:59 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect_utils.h"
#include <signal.h>

static char	*g_heredoc_tempfile = NULL;

void	cleanup_temp_file(char *template)
{
	if (template)
	{
		unlink(template);
		free(template);
	}
}

char	*ft_strappend(char **s1, char *s2)
{
	char	*str;
	char	*tmp;
	size_t	s1_len;
	size_t	s2_len;

	if (*s1)
		s1_len = ft_strlen(*s1);
	else
		s1_len = 0;
	if (s2)
		s2_len = ft_strlen(s2);
	else
		s2_len = 0;
	str = (char *)ft_calloc(sizeof(char), (s1_len + s2_len + 2));
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
	{
		ft_strlcat(str, s2, s1_len + s2_len + 1);
		tmp = str + s1_len + s2_len;
		*tmp++ = '\n';
		*tmp = '\0';
	}
	if (*s1)
		free(*s1);
	if (s2)
		free(s2);
	*s1 = str;
	return (*s1);
}

static void	heredoc_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		if (g_heredoc_tempfile)
		{
			unlink(g_heredoc_tempfile);
			free(g_heredoc_tempfile);
			g_heredoc_tempfile = NULL;
		}
		ft_dprintf(STDOUT_FILENO, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static char	*read_heredoc_content(const char *delimiter)
{
	char				*line;
	char				*content;
	size_t				delim_len;
	struct sigaction	old_sa;
	struct sigaction	new_sa;
	int					original_stdout;

	content = NULL;
	delim_len = ft_strlen(delimiter);
	original_stdout = dup(STDOUT_FILENO);
	if (original_stdout == -1)
		return (NULL);
	
	/* Set up heredoc signal handler */
	sigemptyset(&new_sa.sa_mask);
	new_sa.sa_flags = 0;
	new_sa.sa_handler = heredoc_signal_handler;
	sigaction(SIGINT, &new_sa, &old_sa);
	while (1)
	{
		if (isatty(original_stdout))
			dup2(original_stdout, STDOUT_FILENO);
		line = readline("> ");
		if (!line)
		{
			free(content);
			content = NULL;
			break ;
		}
		if (ft_strlen(line) == delim_len && ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_strappend(&content, line);
		if (!content)
			break ;
	}
	sigaction(SIGINT, &old_sa, NULL);
	close(original_stdout);
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
	g_heredoc_tempfile = template;
	fd = ft_mkstemp(template);
	if (fd < 0)
	{
		print_redirect_error(template, strerror(errno));
		if (template)
			free(template);
		return (-1);
	}
	content = read_heredoc_content(delimiter);
	if (content)
		result = ft_fputs(content, fd);
	else
		result = 0;
	close(fd);
	if (result >= 0)
		result = handle_input_redirect(template);
	cleanup_temp_file(template);
	g_heredoc_tempfile = NULL;
	return (result);
}
