/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc_strings.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 02:00:00 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 04:08:02 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect_utils.h"

static void	_helper_get_string_lengths(char **s1, char *s2, size_t *s1_len,
		size_t *s2_len)
{
	if (*s1)
		*s1_len = ft_strlen(*s1);
	else
		*s1_len = 0;
	if (s2)
		*s2_len = ft_strlen(s2);
	else
		*s2_len = 0;
}

static char	*_helper_allocate_and_handle_failure(char **s1, char *s2,
		size_t total_len)
{
	char	*str;

	str = (char *)ft_calloc(sizeof(char), total_len);
	if (!str)
	{
		if (*s1)
			free(*s1);
		if (s2)
			free(s2);
		return (NULL);
	}
	return (str);
}

static void	_helper_copy_and_append_strings(t_string_append_params *params)
{
	char	*tmp;

	if (*(params->s1))
		ft_strlcpy(params->str, *(params->s1), params->s1_len + 1);
	if (params->s2)
	{
		ft_strlcat(params->str, params->s2,
			params->s1_len + params->s2_len + 1);
		tmp = params->str + params->s1_len + params->s2_len;
		*tmp++ = '\n';
		*tmp = '\0';
	}
}

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
	char					*str;
	size_t					s1_len;
	size_t					s2_len;
	t_string_append_params	params;

	_helper_get_string_lengths(s1, s2, &s1_len, &s2_len);
	str = _helper_allocate_and_handle_failure(s1, s2, s1_len + s2_len + 2);
	if (!str)
		return (NULL);
	params.str = str;
	params.s1 = s1;
	params.s2 = s2;
	params.s1_len = s1_len;
	params.s2_len = s2_len;
	_helper_copy_and_append_strings(&params);
	if (*s1)
		free(*s1);
	if (s2)
		free(s2);
	*s1 = str;
	return (*s1);
}
