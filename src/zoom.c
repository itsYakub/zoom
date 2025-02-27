#include "zoom.h"
#include "zoom-opengl.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	unsigned	tex;
	char		*data;
	void		*wnd;
	void		*rndr;
	t_cam		cam;
	
	data = zoom_screenshot();
	wnd = zoom_init();
	rndr = zoom_render(wnd);
	tex = zoom_texture(zoom_window_w(wnd), zoom_window_h(wnd), data);
	free(data);
	cam = (t_cam) { 0 };
	cam.scl = 1.0f;
	while (!zoom_should_quit(wnd)) {
		if (zoom_mouse_pressed(wnd)) {
			zoom_pan(wnd, &cam);
		}
		if (zoom_mouse_wheel(wnd) != 0.0f) {
			zoom_zoom(wnd, &cam);
		}
		zoom_begin(rndr);
		zoom_begin_camera(rndr, cam);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		zoom_draw_texture(rndr, (t_vec4) { 0.0f, 0.0f, zoom_window_w(wnd), zoom_window_h(wnd) }, (t_vec4) { 0.0f, 0.0f, 1.0f, 1.0f }, tex);
		zoom_end_camera(rndr);
		zoom_end(rndr);
		zoom_swap_buffers(wnd);
		zoom_event_poll(wnd);
	}
	zoom_texture_free(tex);
	zoom_render_quit(rndr);
	zoom_quit(wnd);
}

int	zoom_pan(void *wnd, t_cam *cam) {
	t_vec2	delta;

	delta[0] = zoom_mouse_x(wnd) - zoom_mouse_prev_x(wnd);
	delta[1] = zoom_mouse_y(wnd) - zoom_mouse_prev_y(wnd);
	zoom_vec2_scale(delta, (-1 / cam->scl), delta);
	cam->pos[0] += delta[0];
	cam->pos[1] += delta[1];
	return (1);
}

int	zoom_zoom(void *wnd, t_cam *cam) {
	t_vec2	mouse_world_pos;
	float	scale_factor;
	float	mouse_wheel;
	t_wnd	*wptr;

	wptr = (t_wnd *) wnd;
	zoom_screen_to_world(cam, (t_vec2) { zoom_mouse_x(wnd), zoom_mouse_y(wnd) }, mouse_world_pos);
	cam->off[0] = zoom_mouse_x(wnd);
	cam->off[1] = zoom_mouse_y(wnd);
	cam->pos[0] = mouse_world_pos[0];
	cam->pos[1] = mouse_world_pos[1];
	mouse_wheel = zoom_mouse_wheel(wnd);
	scale_factor = 1.0f + (0.25f * fabsf(mouse_wheel));
	if (mouse_wheel < 0.0f)
		scale_factor = 1.0f / scale_factor;
	cam->scl *= scale_factor;
	wptr->s_input.ptr_wheel = 0.0f;
	return (1);
}
