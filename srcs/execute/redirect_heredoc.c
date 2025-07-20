#include "redirect_utils.h"

char	*create_temp_file(void)
{
	char		*pid_str;
	char		*counter_str;
	char		*temp_part;
	char		*temp_id;
	char		*temp_filename;
	int			fd;
	static int	counter = 0;

	pid_str = ft_itoa(getpid());
	if (!pid_str)
		return (NULL);
	counter_str = ft_itoa(counter++);
	if (!counter_str)
	{
		free(pid_str);
		return (NULL);
	}
	temp_part = ft_strjoin(pid_str, "_");
	free(pid_str);
	if (!temp_part)
	{
		free(counter_str);
		return (NULL);
	}
	temp_id = ft_strjoin(temp_part, counter_str);
	free(temp_part);
	free(counter_str);
	if (!temp_id)
		return (NULL);
	temp_filename = ft_strjoin("/tmp/.minishell_heredoc_", temp_id);
	free(temp_id);
	if (!temp_filename)
		return (NULL);
	fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(temp_filename);
		return (NULL);
	}
	close(fd);
	return (temp_filename);
}

int	handle_heredoc_redirect(const char *delimiter)
{
	char	*temp_filename;
	char	*line;
	int		fd;
	int		result;
	size_t	delim_len;

	if (!delimiter)
		return (-1);
	
	temp_filename = create_temp_file();
	if (!temp_filename)
	{
		print_redirect_error("heredoc", "temporary file creation failed");
		return (-1);
	}

	fd = open(temp_filename, O_WRONLY | O_APPEND);
	if (fd == -1)
	{
		print_redirect_error(temp_filename, strerror(errno));
		cleanup_temp_file(temp_filename);
		free(temp_filename);
		return (-1);
	}

	delim_len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			result = -1;
			break ;
		}
		if (line && ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (ft_strncmp(line, delimiter, delim_len) == 0 && 
			ft_strlen(line) == delim_len)
		{
			free(line);
			result = 0;
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);

	if (result == -1)
	{
		cleanup_temp_file(temp_filename);
		free(temp_filename);
		return (-1);
	}

	result = handle_input_redirect(temp_filename);
	cleanup_temp_file(temp_filename);
	free(temp_filename);
	
	return (result);
}
