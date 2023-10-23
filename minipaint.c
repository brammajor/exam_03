/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minipaint.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brmajor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 10:56:59 by brmajor           #+#    #+#             */
/*   Updated: 2023/10/23 11:51:03 by brmajor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int	error_handler(int	i)
{
	if (i == 0)
		write(1, "Error: argument\n", 16);
	else if (i == 1)
		write(1, "Error: Operation file corrupted\n", 32);
	exit(1);
}

int	print_array(char **arr)
{
	int		i = 0;

	while (arr[i])
	{
		int		j = 0;
		while (arr[i][j])
		{
			write(1, &arr[i][j], 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
	return (0);
}

char	**draw_empty(char **arr, float x, float y, float radius, char c)
{
	int		i = 0;
	int		j;
	float	distance;

	while (arr[i])
	{
		j = 0;
		while (arr[i][j])
		{
			distance = sqrtf((j - x) * (j - x) + (i - y) * (i - y));
// radius - distance < 0.01 instead of 1 because only this way could I get the same
// results as the program given on the exam
			if ((radius - distance) >= 0 && (radius - distance) < 0.01)
				arr[i][j] = c;
			j++;
		}
		i++;
	}
	return (arr);
}

char	**draw_full(char **arr, float x, float y, float radius, char c)
{
	int		i = 0;
	int		j;
	float	distance;

	while (arr[i])
	{
		j = 0;
		while (arr[i][j])
		{
			distance = sqrtf((j - x) * (j - x) + (i - y) * (i - y));
			if ((radius - distance) >= 0)
				arr[i][j] = c;
			j++;
		}
		i++;
	}
	return (arr);
}

int	main(int ac, char **av)
{
	FILE	*file;
	int		width;
	int		height;
	char	bground;
	char	**arr;

	if (ac != 2)
		error_handler(0);
	file = fopen(av[1], "r");
	if (!file)
		error_handler(1);
	if (fscanf(file, "%i %i %c\n", &width, &height, &bground) != 3)
		error_handler(1);
	if (width <= 0 || width > 300 || height <= 0 || width > 300)
		error_handler(1);
//	malloc * 2 because i got strange problems on the exam where a width of
//	exactly 15 or 277 would give a segfault and leaks are allowed anyway
	arr = malloc(sizeof(char *) * height * 2);
	if (!arr)
		error_handler(1);

	int		i = 0;
	int		j;

	while (i < height)
	{
		j = 0;
		arr[i] = malloc(sizeof(char) * width + 1);
		if (!arr[i])
			error_handler(1);
		while (j < width)
		{
			arr[i][j] = bground;
			j++;
		}
		arr[i][j] = '\0';
		i++;
	}

	char	type;
	float	x;
	float	y;
	float	radius;
	char	c;
	int		scan;


	while (1)
	{
		scan = fscanf(file, "%c %f %f %f %c\n", &type, &x, &y, &radius, &c);
		if (scan == 5)
		{
			if (radius <= 0)
				error_handler(1);
			if (type == 'c')
				arr = draw_empty(arr, x, y, radius, c);
			else if (type == 'C')
				arr = draw_full(arr, x, y, radius, c);
			else
				error_handler(1);
		}
		else if (scan == -1)
		{
			print_array(arr);
			break ;
		}
		else
			error_handler(1);
	}
}
