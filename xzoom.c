#include "xzoom.h"
#include "xzoom-opengl.h"

int main(void) {
	void	*wnd;

	wnd = xzoom_init(800, 600);
	while (!xzoom_should_quit(wnd)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		xzoom_swap_buffers(wnd);
		xzoom_event_poll(wnd);
	}
	xzoom_quit(wnd);
}
