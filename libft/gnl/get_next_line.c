/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 21:19:40 by bgoncalv          #+#    #+#             */
/*   Updated: 2022/01/29 17:23:44 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#define MAX_FD 4095

static char	*get_line(char **s)
{
	size_t	l;
	char	*dst;
	char	*tmp;
	l = 0;
	if (!*s)
		return (NULL);
	while ((*s)[l] && (*s)[l] != '\n')
		l++;
	if ((*s)[l] == '\n')
		l++;
	if (!(*s)[l])
		tmp = NULL;
	else
		tmp = ft_strndup(*s + l, ft_strlen(*s + l));
	dst = ft_strndup(*s, l);
	free(*s);
	*s = tmp;
	return (dst);
}

char	*get_next_line(int fd)
{
	static char	*s_left[MAX_FD] = {NULL};
	char		buffer[BUFFER_SIZE + 1];
	size_t		n;

	if (fd < 0 || 4095 < fd || BUFFER_SIZE < 0)
		return (NULL);
	if (s_left[fd] && ft_hasline(s_left[fd]))
		return (get_line(&s_left[fd]));
	n = read(fd, buffer, BUFFER_SIZE);
	buffer[n] = 0;
	while (0 < n && n <= BUFFER_SIZE)
	{
		s_left[fd] = ft_strjoin(s_left[fd], buffer);
		if (!s_left[fd] || ft_hasline(s_left[fd]))
			break ;
		n = read(fd, buffer, BUFFER_SIZE);
		buffer[n] = 0;
	}
	return (get_line(&s_left[fd]));
}
