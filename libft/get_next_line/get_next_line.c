/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linliu <linliu@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:49:08 by linliu            #+#    #+#             */
/*   Updated: 2025/06/12 15:32:19 by linliu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*copy_line(char *stash)
{
	char	*line;
	int		i;

	if (!stash)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	line = ft_strndup_gnl(stash, i);
	if (!line)
		return (NULL);
	return (line);
}

static char	*clean_stash(char *stash)
{
	char	*left;
	char	*new_string;

	if (!stash)
		return (NULL);
	left = ft_strchr_gnl(stash, '\n');
	if (!left)
		return (free(stash), NULL);
	left++;
	if (*left == '\0')
		return (free(stash), NULL);
	new_string = ft_strndup_gnl(left, ft_strlen_gnl(left));
	if (!new_string)
		return (free(stash), NULL);
	free (stash);
	return (new_string);
}

static	char	*join_stash(char *stash, int fd)
{
	char		*buf;
	ssize_t		bytes;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (free(stash), stash = NULL, NULL);
	while (!ft_strchr_gnl(stash, '\n'))
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes == 0)
			break ;
		if (bytes < 0)
			return (free(buf), free(stash), stash = NULL, NULL);
		buf[bytes] = '\0';
		stash = ft_strjoin_gnl(stash, buf);
		if (!stash)
			return (free(buf), NULL);
	}
	free (buf);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char		*stash;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = join_stash(stash, fd);
	if (!stash)
		return (NULL);
	line = copy_line(stash);
	if (!line)
		return (free(stash), stash = NULL, NULL);
	stash = clean_stash(stash);
	return (line);
}
