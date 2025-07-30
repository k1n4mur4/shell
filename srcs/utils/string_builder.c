/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_builder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:44 by kinamura          #+#    #+#             */
/*   Updated: 2025/07/30 19:38:02 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/string_builder.h"

static int	sb_resize(t_string_builder *sb, size_t new_capacity)
{
	char	*new_buffer;

	if (new_capacity <= sb->capacity)
		return (1);
	new_buffer = ft_calloc(new_capacity + 1, sizeof(char));
	if (!new_buffer)
		return (0);
	if (sb->buffer)
	{
		ft_strlcpy(new_buffer, sb->buffer, sb->length + 1);
		free(sb->buffer);
	}
	sb->buffer = new_buffer;
	sb->capacity = new_capacity;
	return (1);
}

t_string_builder	*sb_create(void)
{
	t_string_builder	*sb;

	sb = ft_calloc(1, sizeof(t_string_builder));
	if (!sb)
		return (NULL);
	if (!sb_resize(sb, SB_INITIAL_CAPACITY))
	{
		free(sb);
		return (NULL);
	}
	sb->length = 0;
	return (sb);
}

void	sb_destroy(t_string_builder *sb)
{
	if (!sb)
		return ;
	if (sb->buffer)
		free(sb->buffer);
	free(sb);
}

char	*sb_finalize(t_string_builder *sb)
{
	char	*result;

	if (!sb || !sb->buffer)
		return (NULL);
	result = ft_strdup(sb->buffer);
	sb_destroy(sb);
	return (result);
}

int	sb_append(t_string_builder *sb, const char *str)
{
	size_t	str_len;
	size_t	new_length;

	if (!sb || !str)
		return (0);
	str_len = ft_strlen(str);
	new_length = sb->length + str_len;
	if (new_length >= sb->capacity)
	{
		if (!sb_resize(sb, new_length * SB_GROWTH_FACTOR))
			return (0);
	}
	ft_strlcat(sb->buffer, str, sb->capacity + 1);
	sb->length = new_length;
	return (1);
}