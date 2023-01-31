#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

typedef struct s_data
{
	int width;
	int	height;
	char	c;
} t_data;

typedef struct s_circle
{
	char type;
	float x;
	float y;
	float radius;
	char c;
} t_circle;

int ft_strlen(char *str)
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
int clear_all(FILE *file, char *map)
{
	if (map)
		free(map);
	fclose(file);
	return (1);
}
char *get_map(FILE *file, t_data *data)
{
	int	ret;
	char	*map;
	if ((ret = fscanf(file, "%d %d %c\n", &data->width, &data->height, &data->c)) != 3)
		return (NULL);
	if (data->width <= 0 || data->width > 300 || data->height <= 0 || data->height > 300)
		return (NULL);
	if (!(map = malloc(data->width * data->height)))
		return (NULL);
	memset(map, data->c, data->width * data->height);
	return (map);
}

void	print_map(char *map, t_data *data)
{
	int i;

	i = 0;
	while (i < data->height)
	{
		write(1, map + (i * data->width), data->width);
		write(1, "\n", 1);
		i++;
	}
}

int	get_value(float x, float y, t_circle *cir)
{
	float distance;
	distance = sqrtf(powf(x - cir->x, 2.0) + powf(y - cir->y, 2.0));
	if (distance <= cir->radius)
	{
		if (cir->radius - distance < 1.00000000)
			return (2);
		return (1);
	}
	return (0);
}
void set_circle(char *map, t_data *data, t_circle *cir)
{
	int x;
	int y;
	int ret;

	y = 0;
	while (y < data->height)
	{
		x = 0;
		while (x < data->width)
		{
			ret = get_value((float)x, (float)y, cir);
			if ((ret && cir->type == 'C') || (ret == 2 && cir->type == 'c'))
				map[(y * data->width) + x] = cir->c;
			x++;
		}
		y++;
	}
}

int	set_circles(FILE *file, char *map, t_data *data)
{
	int ret;
	t_circle cir;

	while ((ret = fscanf(file, "%c %f %f %f %c\n", &cir.type, &cir.x, &cir.y, &cir.radius, &cir.c)) == 5)
	{
		if (cir.type != 'c' && cir.type != 'C')
			return (0);
		if (cir.radius <= 0.00000000)
			return (0);
		set_circle(map, data, &cir);
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
		return (error_print("Error: Operation file corrupted\n") && clear_all(file, NULL));
	if (!set_circles(file, map, &data))
		return (error_print("Error: Operation file corrupted\n") && clear_all(file, map));
	print_map(map, &data);
	clear_all(file, map);
	return (0);
}