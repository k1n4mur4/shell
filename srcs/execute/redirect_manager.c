#include "redirect_utils.h"

void	init_redirect_backup(t_redirect_backup *backup)
{
	backup->stdin_backup = -1;
	backup->stdout_backup = -1;
	backup->stderr_backup = -1;
}

int	setup_redirections(t_redirect *redirects, t_redirect_backup *backup)
{
	t_redirect	*current;

	if (!backup)
		return (-1);
	
	backup->stdin_backup = dup(STDIN_FILENO);
	backup->stdout_backup = dup(STDOUT_FILENO);
	backup->stderr_backup = dup(STDERR_FILENO);
	
	current = redirects;
	while (current)
	{
		if (current->type == R_INPUT)
		{
			if (handle_input_redirect(current->filename) == -1)
				return (-1);
		}
		else if (current->type == R_OUTPUT)
		{
			if (handle_output_redirect(current->filename) == -1)
				return (-1);
		}
		else if (current->type == R_APPEND)
		{
			if (handle_append_redirect(current->filename) == -1)
				return (-1);
		}
		else if (current->type == R_HEREDOC)
		{
			if (handle_heredoc_redirect(current->filename) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}

void	restore_redirections(t_redirect_backup *backup)
{
	if (!backup)
		return ;
	
	if (backup->stdin_backup != -1)
	{
		dup2(backup->stdin_backup, STDIN_FILENO);
		close(backup->stdin_backup);
	}
	if (backup->stdout_backup != -1)
	{
		dup2(backup->stdout_backup, STDOUT_FILENO);
		close(backup->stdout_backup);
	}
	if (backup->stderr_backup != -1)
	{
		dup2(backup->stderr_backup, STDERR_FILENO);
		close(backup->stderr_backup);
	}
}