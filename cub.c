/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eismail <eismail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:04:11 by eismail           #+#    #+#             */
/*   Updated: 2025/01/27 10:29:36 by eismail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <stdio.h>
#include "cub3D.h"

// static mlx_image_t* wall;
// static mlx_image_t* player;
// static mlx_image_t *line;

int close_window(int keycode, void *param)
{
	(void) param;
	if (keycode == 53)
		exit(0);
	return (0);
}
int cross(t_game *data)
{
	(void) data;
		exit(0);
	return (0);
}
int	ft_height(char **s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_wedth(char **s)
{
	size_t max_lenght;
	int i;
	
	i = 0;
	max_lenght = ft_strlen(s[0]);
	while (s[i])
	{
		if (ft_strlen(s[i]) > max_lenght)
			max_lenght = ft_strlen(s[i]);
		i++;
	}
	return (max_lenght);
}

void pint(mlx_image_t *img, int h, int w, int color)
{
	int i;
	int j;
	
	i =0;
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			mlx_put_pixel(img, i, j, color);
			j++;
		}
		i++;
	}
}
void chose_angle(t_game *data, char p)
{
	if (p == 'S')
		data->ply.rotation_angle = M_PI / 2;
	if (p == 'N')
		data->ply.rotation_angle = 3 * (M_PI / 2);
	if (p == 'W')
		data->ply.rotation_angle = M_PI;
	if (p == 'E')
		data->ply.rotation_angle = 0;
	data->ply.side_angle = data->ply.rotation_angle + (M_PI /2);
}

int *minimap_size(t_game *data)
{
	int *bounds;
	bounds = malloc(sizeof(int) * 4);
	if (!bounds)
		return (NULL);
	bounds[0] = (data->x - (MINI_W / 2));
	bounds[1] = (data->y - (MINI_H / 2));
	bounds[2] = (data->x + (MINI_W / 2));
	bounds[3] = (data->y + (MINI_H / 2));
	return (bounds);
}

void rander_minimap(t_game *data, bool p)
{
	int i;
	int j;
	int x;
	int y;
	
	i = -1;
	while(data->map[++i])
	{
		j = -1;
		while(data->map[i][++j])
		{
			x = (j * CELL);
			y = (i * CELL);
			if (data->map[i][j] && data->map[i][j] == '1')
				mlx_image_to_window(data->mlx, data->wall, x, y);
			if (data->map[i][j] && (data->map[i][j] == 'S' || data->map[i][j] == 'W' 
				|| data->map[i][j] == 'N' || data->map[i][j] == 'E') && p)
			{
				chose_angle(data, data->map[i][j]);
				mlx_image_to_window(data->mlx, data->player, x, y);
				data->x = x;
				data->y = y;
			}
		}
	}
}
	
void ft_mlx_init(t_game *data)
{
	mlx_image_t *bg;
	data->h = ft_height(data->map);
	data->w = ft_wedth(data->map);
	data->mlx = mlx_init(W, H, "cub", true);
	data->minimap = mlx_new_image(data->mlx, MINI_W, MINI_H);
	mlx_image_to_window(data->mlx, data->minimap, 0, 0);
	data->player = mlx_new_image(data->mlx, PLAYER, PLAYER);
	pint(data->player, PLAYER, PLAYER, 0xFF000000);
	bg = mlx_new_image(data->mlx, W, H);
	mlx_image_to_window(data->mlx, bg, 0, 0);
	pint_bg(bg, 0, 0);
	data->game = mlx_new_image(data->mlx, W, H);
	mlx_image_to_window(data->mlx, data->game, 0, 0);
	data->line = mlx_new_image(data->mlx, W, H);
	mlx_image_to_window(data->mlx, data->line, 0, 0);
	data->wall = mlx_new_image(data->mlx, CELL, CELL);
	pint(data->wall, CELL, CELL, 0xFFFFFF00);
    mlx_set_cursor_mode(data->mlx, MLX_MOUSE_DISABLED);
	rander_minimap(data, true);
	load_animation(data);
}

void draw_line(mlx_image_t *mlx, int x0, int y0, int x1, int y1, int color)
{
	int dy = y1 - y0;
	int dx = x1 - x0;
	int steps = 0;;
	float	x_inc = 0;
	float	y_inc = 0;
	int i = 0;
	float y = y0;
	float x = x0;

	if (abs(dx) > abs(dy))
		steps = abs(dx);
	else 
		steps = abs(dy);
	x_inc = (float)dx / (float)steps;
	y_inc = (float)dy / (float)steps;
	while(i < steps && y >= 0 && x >= 0)
	{
		mlx_put_pixel(mlx, round(x), round(y), color);
		x += x_inc;
		y += y_inc;
		i++;
	}
}

bool phaswall(double x, double y, t_game *data)
{
	int cell_x;
	int cell_y;
	
	if (x < 0 || x > (data->w * CELL) || y < 0 || y > (data->h * CELL))
		return (true);
	if ((size_t)floor(x / CELL) >= ft_strlen(data->map[(size_t)floor(y / CELL)]) )
		return (true);
	int dx = 0; 
	while(dx < PLAYER)
	{
		int dy = 0;
		while (dy < PLAYER)
		{
			cell_x = floor((x + dx) / CELL);
			cell_y = floor((y + dy) / CELL);
			if (data->map[cell_y][cell_x] == '1')
				return (true);
			dy += PLAYER - 1;
		}
		dx += PLAYER - 1;
	}
	return (false);
}

bool haswall(double x, double y, t_game *data)
{
	long cell_x;
	long cell_y;
	
	cell_x = floor(x / CELL);
	cell_y = floor(y / CELL);
	if (x < 0 || x > (data->w * CELL) || y < 0 || y > (data->h * CELL))
		return (true);
	if ((size_t)(cell_x) >= ft_strlen(data->map[cell_y]) )
		return (true);
	if (data->map[cell_y][cell_x] == '1')
		return (true);
	return (false);
}

double distance(double x0, double y0, double x1,double y1)
{
	return (sqrt(((x1 - x0) * (x1 - x0)) + ((y1 - y0) * (y1 - y0))));
}

// bool steping(double nexthitx, double nexthity, t_game *data)
// {

// }

double *wallhit(t_game *data, double xinter, double yinter , double *steps)
{
	double *wallhit;

	wallhit = ft_calloc(sizeof(double), 2);
	while ((yinter >= 0 && xinter >= 0 && yinter < (data->h * CELL) 
		&& xinter < (data->w * CELL) && (size_t)floor(xinter / CELL) 
		< ft_strlen(data->map[(size_t)floor(yinter / CELL)])))
	{
		if (haswall(xinter, yinter, data))
		{
			wallhit[0] = xinter;
			wallhit[1] = yinter;
			break;
		}
		else
		{
			xinter += steps[0];
			yinter += steps[1];
		}
	}
	return (wallhit);
}

double* ft_vertical(t_game *data, double startx, double starty, double angl)
{
	double yinter;
	double xinter;
	double *steps;
	double *verwallhit;

	steps = malloc(sizeof(double) * 2);
	xinter = floor(startx / CELL) * CELL;
	if(data->right)
		xinter += CELL;
	yinter = starty + ((xinter - startx) * tan(angl));
	steps[0] = CELL;
	if (data->left)
		steps[0] *= -1;
	steps[1] = CELL * tan(angl);
	if(data->up && steps[1] > 0)
		steps[1] *= -1;
	if (data->down && steps[1] < 0)
		steps[1] *= -1;
	if (data->left)
		verwallhit = wallhit(data, xinter - 0.000001, yinter, steps);
	else
		verwallhit = wallhit(data, xinter, yinter, steps);
	return (free(steps), verwallhit);
}
double* ft_horisontal(t_game *data, double startx, double starty, double angl)
{
	double yinter;
	double xinter;
	double *steps;
	double *horwallhit;
	
	steps = malloc(sizeof(double) * 2);
	yinter = floor(starty / CELL) * CELL;
	if (data->down)
		yinter += CELL;
	xinter = startx + ((yinter - starty) / tan(angl));
	steps[0] = CELL / tan(angl);
	steps[1] = CELL;
	if (data->up)
		steps[1] *= -1;
	if(data->left && steps[0] > 0)
		steps[0] *= -1;
	if (data->right && steps[0] < 0)
		steps[0] *= -1;
	if (data->up)
		horwallhit = wallhit(data, xinter, yinter - 0.00001, steps);
	else
		horwallhit = wallhit(data, xinter, yinter, steps);
	return (free(steps), horwallhit);
}
void norm_engle(t_game *data, double *angle)
{
	*angle = remainder(*angle , (2 * M_PI));
	if (*angle < 0)
		*angle = (2 * M_PI) + *angle;
	if (*angle > 0 && *angle < M_PI)
		data->down = true;
	else
		data->down = false;
	if (*angle < 0.5 * M_PI || *angle > 1.5 * M_PI)
		data->right = true;
	else
		data->right = false;
	data->left   = !data->right;
	data->up     = !data->down;
}
double *get_distance(t_game *data, double *horwallhit, double *verwallhit)
{
	double *distance_arry;
	
	distance_arry = malloc(sizeof(double) * 2);
	if (!distance_arry)
		return (NULL);
	distance_arry[0] = __INT_MAX__;
	distance_arry[1] = __INT_MAX__;
	if (horwallhit && horwallhit[0] > 0 && horwallhit[1] > 0)
		distance_arry[0] = distance(data->x, data->y, horwallhit[0], horwallhit[1]);
	if (verwallhit && verwallhit[0] > 0 && verwallhit[1] > 0)
		distance_arry[1] = distance(data->x, data->y, verwallhit[0], verwallhit[1]);
	return(distance_arry);
}
double *cmp_hv(t_game *data, double startx, double starty, double angle)
{
	double *wallhit;
	double *distances;
	double *horwallhit;
	double *verwallhit;

	wallhit = malloc(sizeof(double) * 3);
	if (!wallhit)
		return (NULL);
	horwallhit = ft_horisontal(data, startx, starty, angle);
	verwallhit = ft_vertical(data, startx, starty, angle);
	distances = get_distance(data, horwallhit, verwallhit);
	if (distances[0] < distances[1])
	{
		wallhit[0] = horwallhit[0];
		wallhit[1] = horwallhit[1];
		wallhit[2] = rgb(0, 103, 72, 255);
	}
	else 
	{
		wallhit[0] = verwallhit[0];
		wallhit[1] = verwallhit[1];
		wallhit[2] = rgb(238, 216, 186, 255);
	}
	return (free(horwallhit), free(verwallhit), free(distances), wallhit);
}

int rgb(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

void pint_bg(mlx_image_t *img, double x, double y)
{
	double i;
	double j;
	int sky_color;
	int flor_color;
	//yssf
	sky_color = rgb(125, 166, 189, 255);
	flor_color = rgb(117,107,93,255);
	i = x;
	while (i < W)
	{	
		j = y;
		while (j < H)
		{
			if (j < (y + (H / 2)))
				mlx_put_pixel(img, i, j, sky_color);
			else
				mlx_put_pixel(img, i, j, flor_color);
			j++;
		}
		i++;
	}
}
void rectangle(t_game *data, double x, double y, double width, double height)
{
	double i;
	double j;
	if (x < 0)
		x = 0;
    if (y < 0) 
		y = 0;
    if (x + width > data->game->width)
		width = data->game->width - x;
    if (y + height > data->game->height)
		height = data->game->height - y;
	
	i = x;
	while (i < (x + width))
	{	
		j = y;
		while (j < (y + height))
		{
			mlx_put_pixel(data->game, i, j, data->color);
			j++;
		}
		i++;
	}
}
void fill_rays(t_game *data, double *ray, int i, double dis)
{
	double dis_plane;
	
	dis_plane = (W / 2) / tan(FOV_ANGLE / 2);
	data->rays[i].x = ray[0];
	data->rays[i].y = ray[1];
	data->rays[i].wall_height = (CELL / dis) * dis_plane;
	data->rays[i].distance = distance(data->x, data->y, ray[0], ray[1]);
	if (ray[2] == rgb(238, 216, 186, 255))
		data->rays[i].vertical = true;
	else
		data->rays[i].vertical = false;
}

void reander_walls(t_game *data, double **rays)
{
	double dis;
	double dis_plane;
	double wall_height;
	double angle;
	int i;

	i = 0;
	angle = data->ply.rotation_angle - (FOV_ANGLE / 2);
	while (i < NUM_RAYS)
	{
		data->color = rays[i][2];
		data->rays[i].angle = angle;
		dis = distance(data->x, data->y, rays[i][0], rays[i][1]) * cos(angle - data->ply.rotation_angle);
		fill_rays(data, rays[i], i, dis);
		dis_plane = (W / 2) / tan(FOV_ANGLE / 2);
		fill_rays(data, rays[i], i, dis);
		wall_height = (CELL / dis) * dis_plane;
		// rectangle(data, i * WALL_STRIP_WIDTH, (H / 2) - (wall_height/ 2), WALL_STRIP_WIDTH, wall_height);
		i++;
		angle += FOV_ANGLE / NUM_RAYS;
	}
}
void cast_all_rays(t_game *data)
{
	int colm;
	double angle;
	double *rays[NUM_RAYS];

	colm = 0;
	angle = data->ply.rotation_angle - (FOV_ANGLE / 2);
	while (colm < NUM_RAYS)
	{
		norm_engle(data, &angle);
		data->rays[colm].down = data->down;
		data->rays[colm].up = data->up;
		data->rays[colm].left = data->left;
		data->rays[colm].right = data->right;
		rays[colm] = cmp_hv(data, data->x , data->y, angle);
		// draw_line(data->line, data->x, data->y, rays[colm][0], rays[colm][1], 0xFF0000FF);
		colm++;
		angle += FOV_ANGLE / NUM_RAYS;
	}
	reander_walls(data, rays);
	colm = 0;
	while (colm < NUM_RAYS)
	{
		free(rays[colm]);
		colm++;
	}
}
void draw_square(void *img, int startx, int starty, int size, int color) 
{
    int i;
	int j;
	
	i = starty;
    while (i < (starty + size)) 
	{
		j = startx;
        while (j < (startx + size)) 
		{
            mlx_put_pixel(img,j , i, color);
			j++;
		}
		i++;
    }
}

void minimap(t_game *data)
{
	int x;
	int y;
	int *arry;
	size_t cell_x;
	int cell_y;

	cell_x = 0;
	cell_y = 0;
	arry = minimap_size(data);
	y = -1;
	while(++y < MINI_H - CELL)
	{
		x = -1;
		while(++x < MINI_W - CELL && (int)cell_x < arry[2] && cell_y < arry[3])
		{
			cell_x = ((arry[0] + x) / CELL);
			cell_y = ((arry[1] + y) / CELL);
			mlx_put_pixel(data->minimap, x, y, 0x000000CC);
			if (cell_y >= 0 && cell_y < data->h && cell_x >= 0 
				&& cell_x < ft_strlen(data->map[cell_y]) && data->map[cell_y][cell_x] == '1')
				mlx_put_pixel(data->minimap, x, y, 0xFFFFFF99);
		}
	}
	draw_square(data->minimap, MINI_W / 2 , MINI_H / 2, PLAYER, 0xFF0000FF);
	free(arry);
}
void rebiuld(t_game *data)
{
	int mouse_x;
	int mouse_y;
	
	mouse_x = 0;
	mouse_y = 0;
	static int pos_x;
	mlx_get_mouse_pos(data->mlx, &mouse_x, &mouse_y);
	if (!haswall(data->x, data->y, data) && (mlx_is_key_down(data->mlx, MLX_KEY_S) 
		|| mlx_is_key_down(data->mlx, MLX_KEY_D) || mlx_is_key_down(data->mlx, MLX_KEY_A) 
		|| mlx_is_key_down(data->mlx, MLX_KEY_UP) || mlx_is_key_down(data->mlx, MLX_KEY_W) 
		|| mlx_is_key_down(data->mlx, MLX_KEY_DOWN) || mlx_is_key_down(data->mlx, MLX_KEY_RIGHT) 
		|| mlx_is_key_down(data->mlx, MLX_KEY_LEFT) || mouse_x != pos_x))
	{
		data->color = 0;
		rectangle(data, 0, 0, W, H);
		// mlx_delete_image(data->mlx,data->line);
		// data->line = mlx_new_image(data->mlx,W, H);
		// mlx_image_to_window(data->mlx, data->line, 0, 0);
	}
	mlx_delete_image(data->mlx,data->minimap);
	data->minimap = mlx_new_image(data->mlx, MINI_W, MINI_H);
	mlx_image_to_window(data->mlx, data->minimap, 0, 0);
	minimap(data);
	pos_x = mouse_x;
}
void clean_img(t_game *data, char **img_name,mlx_texture_t** texture, mlx_image_t** img)
{
	int i = 0;
	while (i < 28)
	{
		mlx_delete_image(data->mlx, img[i]);
		mlx_delete_texture(texture[i]);
		i++;
	}
	i = 0;
	while(img_name[i])
	{
		free(img_name[i]);
		i++;
	}
	free(img_name);
}
void load_animation(t_game *data)
{
	int i;
	char *img_name;
	char *tmp;
	char *nums = "123456789ABCDE";
	mlx_texture_t *texture[ANIM];
	
	i = 0;
	while (i < ANIM)
	{
		img_name = ft_substr(nums, i, 1);
		tmp = ft_strjoin("a/", img_name);
		free(img_name);
		img_name = ft_strjoin(tmp, ".png");
		texture[i] = mlx_load_png(img_name);
		data->img[i] = mlx_texture_to_image(data->mlx, texture[i]);
		mlx_image_to_window(data->mlx, data->img[i],
			(W/2) - (data->img[i]->width/2) , H - data->img[i]->height);
		data->img[i]->enabled = false;
		i++;
		free(tmp);
		free(img_name);
	}
}

void animation(t_game *data)
{	
	int animation_speed;
	static int current_image;
	
    animation_speed = 2;
	data->frame_counter++;
    if (data->frame_counter % animation_speed == 0)
    {
        data->img[current_image]->enabled = false;
        current_image = (current_image + 1) % ANIM;
        data->img[current_image]->enabled = true;
    }
}
void ft_keys(t_game *data)
{
	data->ply.turn_direction = 0;
	data->ply.side_direction = 0;
	data->ply.walk_direction = 0;
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(data->mlx);
	if (mlx_is_key_down(data->mlx, MLX_KEY_UP) || mlx_is_key_down(data->mlx, MLX_KEY_W))
		data->ply.walk_direction = +1;
	if (mlx_is_key_down(data->mlx, MLX_KEY_DOWN) || mlx_is_key_down(data->mlx, MLX_KEY_S))
		data->ply.walk_direction = (-1);
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
		data->ply.turn_direction = (-1);
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT))
		data->ply.turn_direction = (+1);
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
		data->ply.side_direction = (-1);
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
		data->ply.side_direction = (+1);
	rebiuld(data);
	data->ply.rotation_angle += data->ply.turn_direction * data->ply.rotationSpeed;
	data->ply.side_angle += data->ply.turn_direction * data->ply.rotationSpeed;
	data->ply.movestep = data->ply.walk_direction * data->ply.move_speed;
}

void ft_move(t_game *data)
{
	double newx;
	double newy;

	ft_keys(data);
	if (data->ply.movestep == 0)
	{
		data->ply.movestep = data->ply.side_direction * data->ply.move_speed;
		newx = data->player->instances->x + round(cos(data->ply.side_angle) * data->ply.movestep);
		newy = data->player->instances->y + round(sin(data->ply.side_angle) * data->ply.movestep);
	}
	else
	{
		newx = data->player->instances->x + round(cos(data->ply.rotation_angle) * data->ply.movestep);
		newy = data->player->instances->y + round(sin(data->ply.rotation_angle) * data->ply.movestep);	
	}
	if (!phaswall(newx, newy, data))
	{
		data->player->instances->x = newx;
		data->player->instances->y = newy;
	}
	data->x = data->player->instances->x + (PLAYER / 2);
	data->y = data->player->instances->y + (PLAYER / 2);
}

void mouse_mv(t_game *data)
{
    int x;
    int y;
    static int pos_x;

    mlx_get_mouse_pos(data->mlx, &x, &y);
	if (pos_x != 0)
		data->ply.rotation_angle += (x - pos_x) * 0.002;
	pos_x = x;
	norm_engle(data, &data->ply.rotation_angle);
	data->ply.side_angle = data->ply.rotation_angle + (M_PI /2);
}

void ft_hook(void* param)
{
	t_game* data = param;

	mouse_mv(data);
	ft_move(data);
	animation(data);
	cast_all_rays(data);
	draw_texture(data);
}


t_ply_info init_ply()
{
	t_ply_info ply;
	ply.turn_direction = 0; // -1 if left , +1 if right
	ply.walk_direction = 0; // -1 if back , +1 if front
	ply.rotation_angle = 0;
	ply.side_angle = 0;
	ply.move_speed = 2.0;
	ply.rotationSpeed = 2 * (M_PI / 180);
	ply.movestep = 0;
	return (ply);
}


