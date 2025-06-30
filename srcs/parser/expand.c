#include "parser.h"
#include "env.h"
#include "eval.h"

/* Check if character is part of variable name */
static int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/* Get environment variable value */
static char	*get_env_value(const char *var_name)
{
	t_env	*env_list;
	
	env_list = env(NULL, GET);
	while (env_list)
	{
		if (ft_strcmp(env_list->key, var_name) == 0)
			return (env_list->value ? ft_strdup(env_list->value) : ft_strdup(""));
		env_list = env_list->next;
	}
	return (ft_strdup(""));
}

/* Get special variable value ($?, $$, etc.) */
static char	*get_special_var(char c)
{
	if (c == '?')
		return (ft_itoa(exit_value(GET, 0)));
	else if (c == '$')
		return (ft_itoa(getpid()));
	return (ft_strdup(""));
}

/* Extract variable name from string starting with $ */
static char	*extract_var_name(const char *str, int *advance)
{
	int		i;
	char	*var_name;

	if (!str || str[0] != '$')
		return (NULL);
	if (str[1] == '?' || str[1] == '$')
	{
		*advance = 2;
		var_name = ft_calloc(2, sizeof(char));
		if (var_name)
			var_name[0] = str[1];
		return (var_name);
	}
	i = 1;
	while (str[i] && is_valid_var_char(str[i]))
		i++;
	if (i == 1)
	{
		*advance = 1;
		return (ft_strdup(""));
	}
	*advance = i;
	var_name = ft_calloc(i, sizeof(char));
	if (!var_name)
		return (NULL);
	if (i > 1)
		ft_strlcpy(var_name, &str[1], i);
	return (var_name);
}

/* Remove quotes and return content (no variable expansion for single quotes) */
static char	*expand_single_quoted(const char *word)
{
	int		len;
	char	*result;
	int		i;
	int		j;

	if (!word)
		return (NULL);
	len = ft_strlen(word);
	result = ft_calloc(len + 1, sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (word[i])
	{
		if (word[i] != '\'')
			result[j++] = word[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

/* Expand variables in double-quoted string and remove quotes */
static char	*expand_double_quoted(const char *word)
{
	char	*result;
	char	*temp;
	char	*var_name;
	char	*var_value;
	int		i;
	int		start;
	int		advance;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (word[i])
	{
		if (word[i] == '"')
		{
			i++;
			continue;
		}
		if (word[i] == '$' && word[i + 1])
		{
			var_name = extract_var_name(&word[i], &advance);
			if (var_name && var_name[0])
			{
				if (var_name[0] == '?' || var_name[0] == '$')
					var_value = get_special_var(var_name[0]);
				else
					var_value = get_env_value(var_name);
				temp = ft_strjoin(result, var_value);
				free(result);
				free(var_value);
				result = temp;
				if (!result)
				{
					free(var_name);
					return (NULL);
				}
			}
			else
			{
				temp = ft_strjoin(result, "$");
				free(result);
				result = temp;
				if (!result)
				{
					free(var_name);
					return (NULL);
				}
				advance = 1;
			}
			free(var_name);
			i += advance;
		}
		else
		{
			start = i;
			while (word[i] && word[i] != '$' && word[i] != '"')
				i++;
			if (i > start)
			{
				char *substr = ft_calloc(i - start + 1, sizeof(char));
				if (!substr)
				{
					free(result);
					return (NULL);
				}
				ft_strlcpy(substr, &word[start], i - start + 1);
				temp = ft_strjoin(result, substr);
				free(result);
				free(substr);
				result = temp;
				if (!result)
					return (NULL);
			}
		}
	}
	return (result);
}

/* Expand variables in unquoted string */
static char	*expand_unquoted(const char *word)
{
	char	*result;
	char	*temp;
	char	*var_name;
	char	*var_value;
	int		i;
	int		start;
	int		advance;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (word[i])
	{
		if (word[i] == '$' && word[i + 1])
		{
			var_name = extract_var_name(&word[i], &advance);
			if (var_name && var_name[0])
			{
				if (var_name[0] == '?' || var_name[0] == '$')
					var_value = get_special_var(var_name[0]);
				else
					var_value = get_env_value(var_name);
				temp = ft_strjoin(result, var_value);
				free(result);
				free(var_value);
				result = temp;
				if (!result)
				{
					free(var_name);
					return (NULL);
				}
			}
			else
			{
				temp = ft_strjoin(result, "$");
				free(result);
				result = temp;
				if (!result)
				{
					free(var_name);
					return (NULL);
				}
				advance = 1;
			}
			free(var_name);
			i += advance;
		}
		else
		{
			start = i;
			while (word[i] && word[i] != '$')
				i++;
			if (i > start)
			{
				char *substr = ft_calloc(i - start + 1, sizeof(char));
				if (!substr)
				{
					free(result);
					return (NULL);
				}
				ft_strlcpy(substr, &word[start], i - start + 1);
				temp = ft_strjoin(result, substr);
				free(result);
				free(substr);
				result = temp;
				if (!result)
					return (NULL);
			}
		}
	}
	return (result);
}

/* Process mixed quotes properly */
static char	*expand_mixed_quotes(const char *word)
{
	char	*result;
	char	*temp;
	char	*part;
	int		i;
	int		start;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (word[i])
	{
		start = i;
		if (word[i] == '\'')
		{
			i++;
			while (word[i] && word[i] != '\'')
				i++;
			if (word[i] == '\'')
				i++;
			part = ft_calloc(i - start + 1, sizeof(char));
			if (!part)
			{
				free(result);
				return (NULL);
			}
			ft_strlcpy(part, &word[start], i - start + 1);
			temp = expand_single_quoted(part);
			free(part);
		}
		else if (word[i] == '"')
		{
			i++;
			while (word[i] && word[i] != '"')
				i++;
			if (word[i] == '"')
				i++;
			part = ft_calloc(i - start + 1, sizeof(char));
			if (!part)
			{
				free(result);
				return (NULL);
			}
			ft_strlcpy(part, &word[start], i - start + 1);
			temp = expand_double_quoted(part);
			free(part);
		}
		else
		{
			while (word[i] && word[i] != '\'' && word[i] != '"')
				i++;
			part = ft_calloc(i - start + 1, sizeof(char));
			if (!part)
			{
				free(result);
				return (NULL);
			}
			ft_strlcpy(part, &word[start], i - start + 1);
			temp = expand_unquoted(part);
			free(part);
		}
		if (temp)
		{
			char *new_result = ft_strjoin(result, temp);
			free(result);
			free(temp);
			result = new_result;
			if (!result)
				return (NULL);
		}
	}
	return (result);
}

/* Determine quote context and expand accordingly */
char	*expand_word(const char *word)
{
	int		has_single;
	int		has_double;

	if (!word)
		return (NULL);
	
	has_single = (ft_strchr(word, '\'') != NULL);
	has_double = (ft_strchr(word, '"') != NULL);
	
	/* Check for mixed quotes */
	if (has_single && has_double)
		return (expand_mixed_quotes(word));
	else if (has_single)
		return (expand_single_quoted(word));
	else if (has_double)
		return (expand_double_quoted(word));
	else
		return (expand_unquoted(word));
}

/* Check if word should be expanded based on context */
int	should_expand_in_context(const char *word)
{
	if (!word)
		return (0);
	
	/* Always process words with quotes or $ variables */
	return (ft_strchr(word, '\'') != NULL || ft_strchr(word, '"') != NULL || ft_strchr(word, '$') != NULL);
}

/* Expand word list */
static void	expand_word_list(t_word_list *words)
{
	t_word_list	*current;
	char		*expanded;

	current = words;
	while (current)
	{
		if (current->word && current->word->word && 
			should_expand_in_context(current->word->word))
		{
			expanded = expand_word(current->word->word);
			if (expanded)
			{
				free(current->word->word);
				current->word->word = expanded;
				/* Remove quote and dollar flags after expansion */
				current->word->flags &= ~(W_HASDOLLAR | W_QUOTED);
			}
		}
		current = current->next;
	}
}

/* Expand redirections */
static void	expand_redirections(t_redirect *redirects)
{
	t_redirect	*current;
	char		*expanded;

	current = redirects;
	while (current)
	{
		if (current->filename && should_expand_in_context(current->filename))
		{
			expanded = expand_word(current->filename);
			if (expanded)
			{
				free(current->filename);
				current->filename = expanded;
			}
		}
		current = current->next;
	}
}

/* Recursively expand AST */
void	expand_ast(t_command *ast)
{
	if (!ast)
		return ;
	
	if (ast->type == CM_SIMPLE && ast->simple)
	{
		if (ast->simple->words)
			expand_word_list(ast->simple->words);
		if (ast->simple->redirects)
			expand_redirections(ast->simple->redirects);
	}
	else if (ast->type == CM_PIPE || ast->type == CM_AND || ast->type == CM_OR)
	{
		expand_ast(ast->left);
		expand_ast(ast->right);
	}
}