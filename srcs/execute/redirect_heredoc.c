#include "redirect_utils.h"

/* Generate unique identifier for temp file */
static char	*generate_temp_id(void)
{
	char	*pid_str;
	char	*counter_str;
	char	*temp_part;
	char	*result;
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
	result = ft_strjoin(temp_part, counter_str);
	free(temp_part);
	free(counter_str);
	return (result);
}

/* Create and initialize temp file */
char	*create_temp_file(void)
{
	char	*temp_id;
	char	*temp_filename;
	int		fd;

	temp_id = generate_temp_id();
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
	add_temp_file(temp_filename);
	return (temp_filename);
}

/* Check if line matches delimiter */
static int	is_delimiter_match(const char *line, const char *delimiter)
{
	size_t	delim_len;

	delim_len = ft_strlen(delimiter);
	return (ft_strncmp(line, delimiter, delim_len) == 0 && 
			ft_strlen(line) == delim_len);
}

/* Process single heredoc line */
static int	process_heredoc_line(int fd, const char *delimiter)
{
	char	*line;

	write(STDERR_FILENO, "> ", 2);
	line = NULL;
	if (get_next_line(STDIN_FILENO, &line) <= 0)
	{
		if (line)
			free(line);
		return (-1);
	}
	if (line && ft_strlen(line) > 0 && 
		line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	if (is_delimiter_match(line, delimiter))
	{
		free(line);
		return (1);
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
	return (0);
}

/* Write heredoc content to temp file */
int	write_heredoc_to_temp(const char *delimiter, const char *temp_filename)
{
	int	fd;
	int	result;

	if (!delimiter || !temp_filename)
		return (-1);
	fd = open(temp_filename, O_WRONLY | O_APPEND);
	if (fd == -1)
	{
		print_redirect_error(temp_filename, strerror(errno));
		return (-1);
	}
	while (1)
	{
		result = process_heredoc_line(fd, delimiter);
		if (result != 0)
			break ;
	}
	close(fd);
	return (result == 1 ? 0 : -1);
}

int	handle_heredoc_redirect(const char *delimiter)
{
	char	*temp_filename;
	int		result;

	if (!delimiter)
		return (-1);
	
	temp_filename = create_temp_file();
	if (!temp_filename)
	{
		print_redirect_error("heredoc", 
			"temporary file creation failed");
		return (-1);
	}

	result = write_heredoc_to_temp(delimiter, temp_filename);
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