#include "zoom.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

int		zoom_mouse_pressed(void *wnd) {
	t_wnd	*wptr;

	wptr = (t_wnd *) wnd;
	return (wptr->s_input.ptr_prs);
}

float	zoom_mouse_wheel(void *wnd) {
	float	wheel;
	t_wnd	*wptr;

	wptr = (t_wnd *) wnd;
	wheel = wptr->s_input.ptr_wheel;
	return (wheel);
}

float	zoom_mouse_x(void *wnd) {
	t_wnd	*wptr;

	wptr = (t_wnd *) wnd;
	return (wptr->s_input.ptr_pos[0]);
}

float	zoom_mouse_y(void *wnd) {
	t_wnd	*wptr;

	wptr = (t_wnd *) wnd;
	return (wptr->s_input.ptr_pos[1]);
}

float		zoom_mouse_prev_x(void *wnd) {
	t_wnd	*wptr;

	wptr = (t_wnd *) wnd;
	return (wptr->s_input.ptr_pos_prev[0]);
}

float		zoom_mouse_prev_y(void *wnd) {
	t_wnd	*wptr;

	wptr = (t_wnd *) wnd;
	return (wptr->s_input.ptr_pos_prev[1]);
}

int	zoom_screen_to_world(t_cam *cam, t_vec2 src, t_vec2 dst) {
	t_mat4x4	mat_cam;
	t_mat4x4	mat_cam_inv;
	t_vec4		vec_trans;
	t_vec4		vec_trans_mul;

	zoom_mat4_identity(mat_cam);
	zoom_mat4_trans(
		mat_cam,
		(t_vec3) {
			cam->off[0],
			cam->off[1],
			0.0f
		},
		mat_cam
	);
	zoom_mat4_scale(
		mat_cam,
		(t_vec3) {
			cam->scl,
			cam->scl,
			1.0f
		},
		mat_cam
	);
	zoom_mat4_trans(
		mat_cam,
		(t_vec3) {
			-cam->pos[0],
			-cam->pos[1],
			0.0f
		},
		mat_cam
	);
	memcpy(mat_cam_inv, mat_cam, sizeof(t_mat4x4));
	zoom_mat4_inverse(mat_cam_inv);
	vec_trans[0] = src[0];
	vec_trans[1] = src[1];
	vec_trans[2] = 0.0f;
	vec_trans[3] = 1.0f;
	zoom_mat4_mulv(mat_cam_inv, vec_trans, vec_trans_mul);
	dst[0] = vec_trans_mul[0];
	dst[1] = vec_trans_mul[1];
	return (1);
}
