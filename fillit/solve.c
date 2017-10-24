/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadamant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/19 17:08:34 by sadamant          #+#    #+#             */
/*   Updated: 2017/10/19 17:08:45 by sadamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

// iterate through map and prints a . or the character
// (depending on the tetrimino that it's in)
void    print_map(t_map *map)
{
    size_t      i;

    i = 0;
    while (map->str[i])
    {
        if (i % (map->size) == 0)
        {
            ft_putchar('\n');
            ft_putchar(map->str[i++]);
        }
        else
            ft_putchar(map->str[i++]);
    }
}

t_map	*init_map(t_tetri **tetriminos)
{
    int		i;
    int 	ntet;
	int    	size;
	char	*map_str;
    t_map   *map;

    i = 0;
    ntet = 0;
	size = 2;
    while ((*tetriminos++)->str)
		ntet++;
	while ((size * size) < ntet * 4)
        size++;
	map = ft_memalloc(sizeof(t_map));
	map_str = ft_memalloc(size * size); //not allocating for end /0
	while (i < size * size)
		map_str[i++] = '.';
    map->size = size;
	map->str = map_str;
	return (map);
}

//check if the tetrimino would fit at the map at m
//m iterates through map, t iterates through tetrimino
//at t = 4, m would offset to account for different map sizes
static int      check_fit(t_map *map, t_tetri *tetrimino)
{
    int     m;
    int     t;
    char    *tet_str;
    char    *map_str;

    printf("initiating new check_fit\n");
    t = 0;
    tet_str = tetrimino->str;
    map_str = map->str;
    if (map->m % map->size > map->size - tetrimino->width)
        map->m = map->m + (map->size - (map->m % map->size)); //this is not a good idea.
    // previously = everything was m instead of map->m, but this doesn't work bc map->m is
    // still being incremented by 1 in insert_tetrimino.
    m = map->m;
    while (tet_str[t])
    {
        if (t != 0 && t % 4 == 0)
            m = m + (map->size) - 4;
        printf("tet_str[%d]:[%c], map_str[%d]:[%c]\n", t, tet_str[t], m, map_str[m]);
        if (tet_str[t++] == '#')
        {
            if (map_str[m] == '.')
                m++;
            else
                return (0);
        }
        else
            m++;
    }
    tetrimino->coord = map->m;
    return (1);
}

// copies out the tetrimino to the map at the m where check_fit succeds
static void insert_tetrimino(t_map *map, t_tetri *tetrimino)
{
    int     t;
    int     m;
    char    *tet_str;

    t = 0;
    m = tetrimino->coord;
    tet_str = tetrimino->str;
    while (tet_str[t])
    {
        if (t != 0 && t % 4 == 0)
            m = m + (map->size) - 4;
        if (tet_str[t++] == '#')
            (map->str)[m++] = tetrimino->letter;
        else
            m++;
    }
    // printf("tet[i]'s coordinates: %d\n", tetrimino->coord);
    map->m++;
}

// iterates through map and calls check_fit at every m, if check_fit
// works, copies out tetrimino to the map, else increments m until
// it finds a fit.
static void fit_tetrimino(t_map *map, t_tetri *tetrimino)
{
    int     M;

    M = (int) ft_strlen(map->str) - (int) ft_strlen(tetrimino->str); //last m where it needs to fit
	while (map->m <= M)
	{
        //check fit needs to be able to iterate through the map from l->r to find the next
        //empty space and try to fit it there, not just fitting linearly. (not even w/ backtracking yet)
        if (check_fit(map, tetrimino))
        {
            insert_tetrimino(map, tetrimino);
            return;
        }
        else
            map->m++;
	}
	return;
}

t_map	*solve(t_tetri **tetriminos, t_map *map)
{
    int i;

    i = 0;
    while (tetriminos[i]->str)
    {
        fit_tetrimino(map, tetriminos[i]);
        printf("tetriminos->str[%d]: %s\n", i, tetriminos[i]->str);
        printf("tetriminos->width[%d]: %d\n", i, tetriminos[i]->width);
        printf("map->str: %s\n", map->str);
        i++;
    }
    return (map);
}

//need this so that bc you can't instantiate when you backtrack
t_map    *solve_entry(t_tetri **tetriminos)
{
    t_map   *map;

    map = init_map(tetriminos);
    solve(tetriminos, map);
    print_map(map);
    return (map);
}
