#include "xzoom.h"
#include "xzoom-opengl.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	unsigned	tex;
	char		*data;
	void		*wnd;
	void		*rndr;
	t_cam		cam;
	
	data = xzoom_screenshot();
	wnd = xzoom_init();
	rndr = xzoom_render(wnd);
	tex = xzoom_texture(XZOOM_RESW, XZOOM_RESH, data);
	free(data);
	cam = (t_cam) { 0 };
	cam.scl = 1.0f;
	while (!xzoom_should_quit(wnd)) {
		if (xzoom_mouse_pressed(wnd)) {
			xzoom_pan(wnd, &cam);
		}
		if (xzoom_mouse_wheel(wnd) != 0.0f) {
			xzoom_zoom(wnd, &cam);
		}
		xzoom_begin(rndr);
		xzoom_begin_camera(rndr, cam);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		xzoom_draw_texture(rndr, (t_vec4) { 0.0f, 0.0f, xzoom_window_w(wnd), xzoom_window_h(wnd) }, (t_vec4) { 0.0f, 0.0f, 1.0f, 1.0f }, tex);
		xzoom_end_camera(rndr);
		xzoom_end(rndr);
		xzoom_swap_buffers(wnd);
		xzoom_event_poll(wnd);
	}
	xzoom_texture_free(tex);
	xzoom_render_quit(rndr);
	xzoom_quit(wnd);
}

int	xzoom_pan(void *wnd, t_cam *cam) {
	t_vec2	delta;

	delta[0] = xzoom_mouse_x(wnd) - xzoom_mouse_prev_x(wnd);
	delta[1] = xzoom_mouse_y(wnd) - xzoom_mouse_prev_y(wnd);
	xzoom_vec2_scale(delta, (-1 / cam->scl), delta);
	cam->pos[0] += delta[0];
	cam->pos[1] += delta[1];
	return (1);
}

int	xzoom_zoom(void *wnd, t_cam *cam) {
	t_vec2	mouse_world_pos;
	float	scale_factor;
	float	mouse_wheel;

	xzoom_screen_to_world(cam, (t_vec2) { xzoom_mouse_x(wnd), xzoom_mouse_y(wnd) }, mouse_world_pos);
	cam->pos[0] = mouse_world_pos[0];
	cam->pos[1] = mouse_world_pos[1];
	cam->off[0] = xzoom_mouse_x(wnd);
	cam->off[1] = xzoom_mouse_y(wnd);
	mouse_wheel = xzoom_mouse_wheel(wnd);
	scale_factor = 1.0f + (0.25f * fabsf(mouse_wheel));
	if (mouse_wheel < 0.0f)
		scale_factor = 1.0f / scale_factor;
	cam->scl *= scale_factor;
	return (1);
}
