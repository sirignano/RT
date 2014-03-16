/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeijoo <cfeijoo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/02 17:42:13 by cfeijoo           #+#    #+#             */
/*   Updated: 2014/03/16 01:18:54 by cfeijoo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include <camera.h>
# include <keyboard.h>
# include <object.h>
# include <matter.h>

# include <pthread.h>

# define VIEWPLANE_PLOT 1000

typedef struct				s_scene
{
	t_camera				camera;
	unsigned int			view_width;
	unsigned int			view_height;
	float					diaphragm;
	int						background_color;
	t_matter				*matters;
	t_object				*objects;
	t_light					*lights;
}							t_scene;

typedef struct				s_env
{
	void					*mlx;
	void					*win;
	void					*img;
	int						bpp;
	int						size_line;
	int						endian;
	int						*data;

	t_scene					*scene;
	t_light_color			*rendering;

	t_pressedkeys			pressed_keys;
	t_object				*selected_object;
	int						pressed_mouse;
	int						last_mouse_x;
	int						last_mouse_y;
	int						block_events;

	int						refresh_image;

	// pthread_t				*render_threads;
	pthread_t				render_thread1;
	// pthread_t				render_thread3;
	// pthread_t				render_thread4;

}							t_env;

typedef struct				s_thread_input
{
	int						x;
	int						y;
	int						width;
	int						height;
	t_env					*env;
}							t_thread_input;


int			update_image(t_env *env);
void		*throw_view_plane(void *env);

void		init_pressed_keys(t_pressedkeys *keys);
int			keypress_hook(int keycode, t_env *env);
void		check_pressed_keys(t_env *env, t_pressedkeys *keys);
int			keyrelease_hook(int keycode, t_env *env);

t_matter	*get_matter(t_env *env, char *name);
void		add_matter(t_env *env, t_matter *matter);
t_matter	*new_matter(char *name);

void		load_matter_object(t_object *object, t_matter *matter);
void		add_object(t_scene *scene, t_object *object);
t_object	*new_object(int type);
void		remove_object(t_scene *scene, t_object *object);

#endif
