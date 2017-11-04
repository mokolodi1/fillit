/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadamant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/19 17:08:34 by sadamant          #+#    #+#             */
/*   Updated: 2017/10/25 18:13:09 by sadamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

//check if the tetrimino would fit at the map at m
//m iterates through map, t iterates through tetrimino
//at t = 4, m would offset to account for different map sizes
static int      check_fit(t_map *map, t_tet *tetrimino)
{
    int     m;
    int     t;

    t = 0;
    m = map->m;
    while (tetrimino->str[t])
    {
        if (t != 0 && t % 4 == 0)
            m = m + (map->size) - 4;
        if (tetrimino->str[t++] == '#')
        {
            if (map->str[m] == '.')
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

//copies out the tetrimino to the map at the m where check_fit succeds
static void insert_tetrimino(t_map *map, t_tet *tetrimino)
{
    int     t;
    int     m;

    t = 0;
    m = tetrimino->coord;
    while (tetrimino->str[t])
    {
        if (t != 0 && t % 4 == 0)
            m = m + (map->size) - 4;
        if (tetrimino->str[t++] == '#')
            (map->str)[m] = tetrimino->letter;
        m++;
    }
}

// clear previous tet
static void clear_tetrimino(t_map *map, t_tet *tetrimino)
{
    int t;
    int m;

    t = 0;
    m = tetrimino->coord;
    while (tetrimino->str[t])
    {
        if (t != 0 && t % 4 == 0)
            m = m + (map->size) - 4;
        if (tetrimino->str[t] == '#')
        {
            (map->str)[m] = '.';
        }
        m++;
        t++;
    }
}

// iterates through map and calls check_fit at every m, if check_fit
// works, copies out tetrimino to the map, else increments m until
// it finds a fit.
static int  fit_tetrimino(t_map *map, t_tet *tetrimino)
{
    int     M;

    M = (int) ft_strlen(map->str) - (int) ft_strlen(tetrimino->str); //last m where it needs to try fitting
    map->m = tetrimino->coord + 1; //set initial t->coord to -1, and so if there's already a slot, it increments it by one (for the recursion!)
	while (map->m <= M)
	{
        if (map->m % map->size > map->size - tetrimino->width)
            map->m = map->m + (map->size - (map->m % map->size));
        if (check_fit(map, tetrimino))
        {
            insert_tetrimino(map, tetrimino);
            return (1);
        }
        map->m++;
	}
	return (0);
}

//clears a tetrimino and tries to refit that tetrimino further along the
//map. if you can't fit that tet any further to the right, it reinserts it
//at the same spot and returns 0.
static int refit_tetrimino(t_map *map, t_tet *tetrimino)
{
    clear_tetrimino(map, tetrimino);
    return (fit_tetrimino(map, tetrimino));
}

static void  reset_coordinates(t_tet **tetriminos, int j, int i)
{
    while (j != i)
        tetriminos[++j]->coord = -1;
}

//i = current tet, j = current backtrack movement.
//if you can fit the current tet in, you recurse and fit the next tet in.
//if you can't fit the current tet in, instantiate backtracking.
int         solve(t_tet **tetriminos, t_map *map, int i)
{
    int j;

    if (tetriminos[i]->str == NULL)
        return (1);
    if (ft_strlen(tetriminos[i]->str) > ft_strlen(map->str))
        return (0);
    while (!fit_tetrimino(map, tetriminos[i]))
    {
        j = i - 1;
        //tries to refit previous tetriminos until one moves to the right, and then tries to solve the current one again.
        while (!refit_tetrimino(map, tetriminos[j]))
        {
            if (--j < 0) //moved all tets to the right and still can't find a match.
                return (0);
        }
        //reset all the coordinates of the tets up until that point, and try to refit all tets.
        reset_coordinates(tetriminos, j, i);
        return (solve(tetriminos, map, ++j));
    }
    return (solve(tetriminos, map, ++i));
}

//need this so that bc you can't instantiate when you backtrack
t_map    *solve_entry(t_tet **tetriminos)
{
    int     i;
    t_map   *map;

    i = 0;
    map = init_map(tetriminos);
    while (!solve(tetriminos, map, i))
    {
        map = expand_map(map);
        reset_coordinates(tetriminos, -1, 25);
    }
    print_map(map);
    return (map);
}
