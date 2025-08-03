/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_builder.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kinamura <kinamura@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:48:44 by kinamura          #+#    #+#             */
/*   Updated: 2025/08/04 03:28:31 by kinamura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_BUILDER_H
# define STRING_BUILDER_H

# include "libft.h"
# include <stdlib.h>

# define SB_INITIAL_CAPACITY 64
# define SB_GROWTH_FACTOR 2

typedef struct s_string_builder
{
	char	*buffer;
	size_t	length;
	size_t	capacity;
}	t_string_builder;

/* String builder management */
t_string_builder	*sb_create(void);
void				sb_destroy(t_string_builder *sb);
char				*sb_finalize(t_string_builder *sb);

/* String operations */
int					sb_append(t_string_builder *sb, const char *str);
int					sb_append_char(t_string_builder *sb, char c);
int					sb_join(t_string_builder *sb, const char *str1,
						const char *str2);

#endif