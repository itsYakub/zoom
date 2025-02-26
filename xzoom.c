#include "xzoom.h"
#include "xzoom-opengl.h"

#include <stdlib.h>

int main(void) {
	unsigned	texture;
	char		*data;
	void		*wnd;
	void		*rndr;
	
	data = xzoom_screenshot();
	wnd = xzoom_init();
	rndr = xzoom_render(wnd);
	texture = xzoom_texture(XZOOM_RESW, XZOOM_RESH, data);
	free(data);
	while (!xzoom_should_quit(wnd)) {
		xzoom_begin(rndr);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		xzoom_draw_texture(rndr, (t_vec4) { 0.0f, 0.0f, xzoom_window_w(wnd), xzoom_window_h(wnd) }, (t_vec4) { 0.0f, 0.0f, 1.0f, 1.0f }, texture);
		xzoom_end(rndr);
		xzoom_swap_buffers(wnd);
		xzoom_event_poll(wnd);
	}
	xzoom_texture_free(texture);
	xzoom_render_quit(rndr);
	xzoom_quit(wnd);
}
