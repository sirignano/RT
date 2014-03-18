/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeijoo <cfeijoo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/02 14:30:35 by cfeijoo           #+#    #+#             */
/*   Updated: 2014/03/18 18:34:00 by cfeijoo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_print.h>
#include <ft_math.h>
#include <ft_colors.h>
#include <ft_convert.h>
#include <ft_memory.h>
#include <math.h>
#include <mlx.h>
#include <X.h>
#include <rt.h>
#include <ray.h>
#include <parser.h>
#include <stdlib.h>
#include <mouse.h>
#include <interpreter.h>
#include <pthread.h>

/*
**	Development Libraries
*/
#include <stdio.h>
#include <tests.h>

/*
**	This is just for development, render dimensions will be parsed in File
*/
#define RENDER_WIDTH			1100
#define RENDER_HEIGHT			670


static int			view_loop(t_env *env)
{

	if (is_one_key_pressed(&env->pressed_keys))
	{
		env->refresh_image = 1;
		check_pressed_keys(env, &env->pressed_keys);
	}
	if (!env->running_threads && !env->block_render && env->refresh_image)
		update_image(env);
	if (!env->running_threads)
	{
		update_render_cam(&env->scene->render_cam, &env->scene->camera);
		if (!env->pressed_keys.shift && env->refresh_image)
			render_to_image(env);
		else
			mlx_put_image_to_window(env->mlx, env->win, env->img, 0, 0);
		env->block_render = 0;
	}
	return (0);
}

t_ray				get_ray_from_point(t_env *env, int i, int j)
{
	t_ray			ray;

	ray.origin.x = env->scene->render_cam.origin.x;
	ray.origin.y = env->scene->render_cam.origin.y;
	ray.origin.z = env->scene->render_cam.origin.z;
	ray.direction.x = env->scene->render_cam.x_axis.x;
	ray.direction.y = env->scene->render_cam.x_axis.y;
	ray.direction.z = env->scene->render_cam.x_axis.z;
	i -= env->scene->view_width / 2;
	j -= env->scene->view_height / 2;
	ray.direction.x -= (env->scene->render_cam.y_axis.x / VIEWPLANE_PLOT) * i;
	ray.direction.y -= (env->scene->render_cam.y_axis.y / VIEWPLANE_PLOT) * i;
	ray.direction.z -= (env->scene->render_cam.y_axis.z / VIEWPLANE_PLOT) * i;
	ray.direction.x -= (env->scene->render_cam.z_axis.x / VIEWPLANE_PLOT) * j;
	ray.direction.y -= (env->scene->render_cam.z_axis.y / VIEWPLANE_PLOT) * j;
	ray.direction.z -= (env->scene->render_cam.z_axis.z / VIEWPLANE_PLOT) * j;
	return (ray);
}

int					update_image(t_env *env)
{
	unsigned int	i;
	unsigned int	j;
	t_thread_input	*input;

	j = 0;
	while (j < RENDER_SPLIT)
	{
		i = 0;
		while (i < RENDER_SPLIT)
		{
			input = (t_thread_input*)malloc(sizeof(*input));
			input->env = env;
			input->x1 = i * (env->scene->view_width / RENDER_SPLIT);
			input->y1 = j * (env->scene->view_height / RENDER_SPLIT);
			input->x2 = env->scene->view_width - (RENDER_SPLIT - 1 - i)
						* (env->scene->view_width / RENDER_SPLIT);
			input->y2 = env->scene->view_width - (RENDER_SPLIT - 1 - j)
						* (env->scene->view_height / RENDER_SPLIT);
			input->thread_number = j * RENDER_SPLIT + i;
			create_render_thread(env, input);
			env->refresh_image = 0;
			i++;
		}
		j++;
	}
	return (0);
}

int					main(int argc, char **argv)
{
	t_env			env;

	if (argc == 2)
	{
		parse_file(argv[1]);
		return (0);
	}
	env.scene = (t_scene*)malloc(sizeof(t_scene));

	env.scene->view_width = RENDER_WIDTH;
	env.scene->view_height = RENDER_HEIGHT;

	env.selected_object = NULL;
	env.pressed_mouse = 0;
	env.running_threads = 0;
	env.block_render = 0;
	env.refresh_image = 1;

	env.scene->background_color = 0xFF000000;
	env.scene->diaphragm = 1.0;
	env.scene->matters = NULL;

	env.interpreter_thread = 0;
	env.block_events = 0;

	env.render_threads = (pthread_t*)ft_memalloc(RENDER_SPLIT * RENDER_SPLIT
						* sizeof(pthread_t));
	env.rendering = (t_light_color*)ft_memalloc(env.scene->view_width
					* env.scene->view_height * sizeof(t_light_color));

	env.mlx = mlx_init();
	env.win = mlx_new_window(env.mlx, env.scene->view_width,
				env.scene->view_height, "RT");
	env.img = mlx_new_image(env.mlx, env.scene->view_width,
				env.scene->view_height);
	env.data = (int*)mlx_get_data_addr(env.img, &(env.bpp),
				&(env.size_line), &(env.endian));

	init_cam(&env.scene->camera, 0, 0, 0);
	init_cam_angle(&env.scene->camera, 0, 0);
	create_test_objects(env.scene);
	init_pressed_keys(&env.pressed_keys);

	create_interpreter_thread(&env);

	mlx_expose_hook(env.win, view_loop, &env);
	mlx_hook(env.win, KeyPress, KeyPressMask, keypress_hook, &env);
	mlx_hook(env.win, KeyRelease, KeyReleaseMask, keyrelease_hook, &env);
	mlx_hook(env.win, ButtonPress, ButtonPressMask, mousepress_ev, &env);
	mlx_hook(env.win, ButtonRelease, ButtonReleaseMask, mouserelease_ev, &env);
	mlx_hook(env.win, MotionNotify, PointerMotionMask, motionnotify_ev, &env);
	mlx_loop_hook(env.mlx, view_loop, &env);
	mlx_loop(env.mlx);
	return (0);
}
