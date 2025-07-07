#include "parser.h"
#include "env.h"
#include "eval.h"

char	*expand_single_quoted(const char *word)
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

char	*expand_double_quoted(const char *word)
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

char	*expand_unquoted(const char *word)
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

char	*expand_mixed_quotes(const char *word)
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
			if (!temp)
			{
				free(result);
				return (NULL);
			}
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
			if (!temp)
			{
				free(result);
				return (NULL);
			}
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
			if (!temp)
			{
				free(result);
				return (NULL);
			}
		}
		char *new_result = ft_strjoin(result, temp);
		free(result);
		free(temp);
		result = new_result;
		if (!result)
			return (NULL);
	}
	return (result);
}