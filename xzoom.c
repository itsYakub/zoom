#include "xzoom.h"
#include "xzoom-opengl.h"

int main(void) {
	void	*wnd;
	void	*rndr;

	wnd = xzoom_init(XZOOM_RESW, XZOOM_RESH);
	rndr = xzoom_render(wnd);
	while (!xzoom_should_quit(wnd)) {
		xzoom_begin(rndr);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		xzoom_draw_quad(
			rndr, 
			(float [3]) { 0.0f, 0.0f },
			(float [3]) { 1.0f, 0.0f },
			(float [3]) { 0.0f, 1.0f },
			(float [3]) { 1.0f, 1.0f },
			(float [4]) { 1.0f, 1.0f, 1.0f, 1.0f },
			(float [4]) { 0.0f, 0.0f, 1.0f, 1.0f },
			xzoom_default_texture(rndr)
		);
		xzoom_end(rndr);
		xzoom_swap_buffers(wnd);
		xzoom_event_poll(wnd);
	}
	xzoom_render_quit(rndr);
	xzoom_quit(wnd);
}
