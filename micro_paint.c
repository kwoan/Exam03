#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

typedef struct s_data
{
	int		width;
	int		height;
	char	c;
} t_data;

typedef struct s_rectangle
{
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	c;
} t_rectangle;

int	ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	error_print(char *str)
{
	write(1, str, ft_strlen(str));
	return (1);
}

int	free_all(FILE *file, char *map)
{
	if (map)
		free(map);
	fclose(file);
	return (1);
}

char *get_map(FILE *file, t_data *data)
{
	int 	ret;
	char	*map;
	if ((ret = fscanf(file, "%d %d %c\n", &data->width, &data->height, &data->c)) != 3)
		return (NULL);
	if (data->width <= 0 || data->height <= 0 || data->width > 300 || data->height > 300)
		return (NULL);
	if (!(map = malloc(data->width * data->height)))
		return (NULL);
	memset(map, data->c, data->width * data->height);
	return (map);
}

void	draw_map(t_data *data, char *map)
{
	int i;

	i = 0;
	while (i < data->height)
	{
		write (1, map + (i * data->width), data->width);
		write (1, "\n", 1);
		i++;
	}
}

int	get_value(float x, float y, t_rectangle *rec)
{
	if (x < rec->x || y < rec->y || rec->x + rec->width < x || rec->y + rec->height < y)
		return (0);
	if (x - rec->x < 1.00000000 ||  y - rec->y < 1.00000000 || rec->x + rec->width - x < 1.00000000 || rec->y + rec->height - y < 1.00000000)
		return (2);
	return (1);
}

void	set_rectangle(t_data *data, char *map, t_rectangle *rec)
{
	int y;
	int x;
	int ret;

	y = 0;
	while (y < data->height)
	{
		x = 0;
		while (x < data->width)
		{
			ret = get_value((float)x, (float)y, rec);
			if ((ret == 2 && rec->type == 'r') || (ret && rec->type == 'R'))
				map[(y * data->width) + x] = rec->c;
			x++;
		}
		y++;
	}
}

int	set_rectangles(FILE *file, t_data *data, char *map)
{
	t_rectangle	rec;
	int			ret;

	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &rec.type, &rec.x, &rec.y, &rec.width, &rec.height, &rec.c)) == 6)
	{
		if (rec.type != 'r' && rec.type != 'R')
			return (0);
		if (rec.width <= 0.00000000 || rec.height <= 0.00000000)
			return (0);
		set_rectangle(data, map, &rec);
	}
	if (ret != -1)
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	FILE	*file;
	char	*map;
	t_data	data;

	if (argc != 2)
		return (error_print("Error: argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (error_print("Error: Operation file corrupted\n"));
	if (!(map = get_map(file, &data)))
		return (error_print("Error: Operation file corrupted\n") && free_all(file, NULL));
	if (!set_rectangles(file, &data, map))
		return (error_print("Error: Operation file corrupted\n") && free_all(file, map));
	draw_map(&data, map);
	free_all(file, map);
	return (0);
}