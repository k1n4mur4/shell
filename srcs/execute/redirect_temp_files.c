#include "redirect_utils.h"

static t_temp_file	*g_temp_files = NULL;

void	add_temp_file(const char *filename)
{
	t_temp_file	*new_file;
	
	if (!filename)
		return ;
	
	new_file = ft_calloc(1, sizeof(t_temp_file));
	if (!new_file)
		return ;
	
	new_file->filename = ft_strdup(filename);
	if (!new_file->filename)
	{
		free(new_file);
		return ;
	}
	
	new_file->next = g_temp_files;
	g_temp_files = new_file;
}

void	remove_temp_file(const char *filename)
{
	t_temp_file	*current;
	t_temp_file	*prev;
	
	if (!filename)
		return ;
	
	current = g_temp_files;
	prev = NULL;
	
	while (current)
	{
		if (ft_strcmp(current->filename, filename) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				g_temp_files = current->next;
			
			free(current->filename);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	cleanup_all_temp_files(void)
{
	t_temp_file	*current;
	t_temp_file	*next;
	
	current = g_temp_files;
	while (current)
	{
		next = current->next;
		unlink(current->filename);
		free(current->filename);
		free(current);
		current = next;
	}
	g_temp_files = NULL;
}

void	init_temp_file_cleanup(void)
{
	atexit(cleanup_all_temp_files);
}

void	cleanup_temp_file(const char *temp_filename)
{
	if (temp_filename)
	{
		unlink(temp_filename);
		remove_temp_file(temp_filename);
	}
}