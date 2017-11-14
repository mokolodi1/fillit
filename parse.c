/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tetrimino_file.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgaia <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 19:14:14 by rgaia             #+#    #+#             */
/*   Updated: 2017/11/07 11:49:45 by sadamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

// not handling invalid block types - unconnected #s, wrong number of #s
// what if there are 0 blocks?
static int	check_valid_format(char *buf)
{
	int		i;
	int		row_len;
	int		column_len;

	i = -1;
	row_len = 0;
	column_len = 0;
	while (buf[++i])
	{
		if (row_len < 4 && VALID_CHAR(buf[i]))
			row_len++;
		else if (row_len == 4 && buf[i] == '\n')
		{
			row_len = 0;
			column_len++;
			if (column_len == 4 && buf[i + 1] == '\n')
			{
				column_len = 0;
				i++;
			}
		}
		else
			return (0);
	}
	// why can't the last two characters be the same?
	return (column_len == 4 && buf[i - 1] != buf[i - 2]);
}

// why not just exit from this function instead of returning back to the main?
int			parse_tetrimino_file(int fd, char *buf)
{
	int		buf_size;

	// what if the entire file isn't read?
	// why is there BUF_SIZE and bug_size?
	buf_size = read(fd, buf, BUF_SIZE);
	close(fd);
	// wtf are these magic numbers
	if (buf_size > 545 || buf_size < 20)
		return (0); // no visible error?
	buf[buf_size] = '\0';
	return (check_valid_format(buf));
}
