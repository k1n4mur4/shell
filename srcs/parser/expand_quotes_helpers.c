/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 04:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:06:31 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "expand_quotes_helpers2.h"

char	*get_variable_value(char *var_name)
{
	char	*value;

	if (ft_strcmp(var_name, "?") == 0)
		return (get_special_var('?'));
	else if (ft_strcmp(var_name, "$") == 0)
		return (get_special_var('$'));
	value = get_env_value(var_name);
	if (!value)
		return (ft_strdup(""));
	return (value);
}

char	*append_variable(char *result, const char *word, int *i)
{
	char	*var_name;
	char	*var_value;
	char	*temp;
	int		advance;

	var_name = extract_var_name(word + *i, &advance);
	if (!var_name)
	{
		(*i)++;
		return (result);
	}
	var_value = get_variable_value(var_name);
	if (!var_value)
	{
		free(var_name);
		(*i)++;
		return (result);
	}
	temp = ft_strjoin(result, var_value);
	free(result);
	free(var_name);
	free(var_value);
	*i += advance;
	return (temp);
}

char	*append_literal(char *result, const char *word, int *i)
{
	char	temp_str[2];
	char	*new_result;

	temp_str[0] = word[*i];
	temp_str[1] = '\0';
	new_result = ft_strjoin(result, temp_str);
	free(result);
	(*i)++;
	return (new_result);
}

char	*process_double_quoted_char(char *result, const char *word, int *i)
{
	if (word[*i] == '$' && word[*i + 1])
		return (append_variable(result, word, i));
	else
		return (append_literal(result, word, i));
}

char	*process_unquoted_char(char *result, const char *word, int *i)
{
	if (word[*i] == '$' && word[*i + 1])
		return (append_variable(result, word, i));
	else
		return (append_literal(result, word, i));
}
