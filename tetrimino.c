/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tetrimino.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadamant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 12:25:10 by sadamant          #+#    #+#             */
/*   Updated: 2017/11/07 12:30:06 by sadamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

int			count_hash(char *t)
{
	int count;

	count = 0;
	while (*t)
	{
		if (*t++ == '#')
			count++;
	}
	if (count != 4)
		return (0);
	return (1);
}

int			connections(char *t)
{
	int	i;
	int	connect;

	i = 0;
	connect = 0;
	while (t[i])
	{
		if (t[i] == '#')
		{
			if ((i + 1 < 20) && t[i + 1] == '#')
				connect++;
			if ((i - 1 >= 0) && t[i - 1] == '#')
				connect++;
			if ((i + 5 < 20) && t[i + 5] == '#')
				connect++;
			if ((i - 5 >= 0) && t[i - 5] == '#')
				connect++;
		}
		i++;
	}
	return (connect == 6 || connect == 8);
}

/*
** input: unreduced tetrimino string
** checks the coordinate of the # to see if it's a min or a max
** output: width of the tetrimino
*/

int			tet_width(char *t)
{
	int	x;
	int l;
	int r;

	x = 0;
	l = 4;
	r = 0;
	while (*t)
	{
		if (x % 5 == 0)
			x = 0;
		if (*t++ == '#')
		{
			if (x < l)
				l = x;
			if (x > r)
				r = x;
		}
		x++;
	}
	return (r - l + 1);
}

/*
** find the leftmost relevant index of the tetrimino
*/

// wtf are these variables? doesn't row col work better here?
static int	leftmost(char *t)
{
	int	x;
	int y;
	int l;

	x = 0;
	y = (ft_strchr(t, '#') - t) / 5;
	l = 4;
	while (*t)
	{
		if (x % 5 == 0)
			x = 0;
		if (*t++ == '#' && x < l)
			l = x;
		x++;
	}
	return (l + y * 5);
}

char		*reduce_tetrimino(char *t)
{
	int		i;
	int		count;
	char	*reduced;
	char	*reducedcpy;

	i = leftmost(t);
	count = 0;
	// why 14?
	reduced = (char *)malloc(sizeof(char) * 14);
	reducedcpy = reduced;
	while (t[i] && count < 4)
	{
		if (t[i] == '\n')
			i++;
		if (t[i] == '#')
			count++;
		*reduced++ = t[i++];
	}
	*reduced = '\0';
	return (reducedcpy);
}
