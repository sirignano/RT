#include <camera.h>
#include <ft_math.h>

void			cam_rot_z(t_camera *camera, float angle)
{
	vect_rot_z(&camera->x_axis, angle);
	vect_rot_z(&camera->y_axis, angle);
	vect_rot_z(&camera->z_axis, angle);
}

void			cam_rot_x(t_camera *camera, float angle)
{
	vect_rot_x(&camera->x_axis, angle);
	vect_rot_x(&camera->y_axis, angle);
	vect_rot_x(&camera->z_axis, angle);
}

void			cam_rot_y(t_camera *camera, float angle)
{
	vect_rot_y(&camera->x_axis, angle);
	vect_rot_y(&camera->y_axis, angle);
	vect_rot_y(&camera->z_axis, angle);
}

void			cam_translate(t_camera *camera, float x, float y, float z)
{
	camera->origin.x += x;
	camera->origin.y += y;
	camera->origin.z += z;
}

void			cam_move_to(t_camera *camera, float x, float y, float z)
{
	camera->origin.x = x;
	camera->origin.y = y;
	camera->origin.z = z;
}