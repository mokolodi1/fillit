/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgaia <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 13:17:42 by rgaia             #+#    #+#             */
/*   Updated: 2017/11/07 12:31:37 by sadamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// "It's really long bc my code is shit" -- what?
// Things could be named a tiny bit better, but this is really well written
// overall. (Tiny honestly means tiny and isn't trying to make you feel
// better.) Who told you your code is shit? You literally have helpful
// comments and well-named function names! 

#include "fillit.h"

static void		exit_error(char *str)
{
	ft_putendl(str);
	exit(EXIT_FAILURE);
}

int				main(int argc, char **argv)
{
	int			fd;
	t_tet		**tetriminos;
	char		*buf;

	// if malloc fails?
	buf = ft_memalloc(BUF_SIZE);
	if (argc != 2)
		exit_error("usage: ./fillit source_file");
	if (!(fd = open(argv[1], O_RDONLY)))
		exit(EXIT_FAILURE);
	if (!(parse_tetrimino_file(fd, buf)))
		exit_error("error");
	if (!(tetriminos = insert_array(buf)))
		exit_error("error");
	solve(tetriminos);
	// doesn't matter if memory is "leaked" at end
	ft_memdel((void**)&tetriminos);
	ft_memdel((void**)&buf);
	return (0);
}
