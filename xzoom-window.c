#include "xzoom.h"

#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <GL/glx.h>

typedef struct s_wnd {
	void		*dsp;
	void		*ctx;
	unsigned	id;
	unsigned	quit;
	Atom		wm_quit;
}	t_wnd;

static int	__xzoom_fullscreen(void *);

static int	vinf_attr[] = {
	GLX_USE_GL,
	GLX_DOUBLEBUFFER,
	GLX_DEPTH_SIZE,         24,                            
	GLX_RED_SIZE,           8,
	GLX_GREEN_SIZE,         8,
	GLX_BLUE_SIZE,          8,
	GLX_ALPHA_SIZE,         8,
	GLX_X_VISUAL_TYPE,      GLX_TRUE_COLOR,
	GLX_RENDER_TYPE,        GLX_RGBA_BIT,
	GLX_DRAWABLE_TYPE,      GLX_WINDOW_BIT,
	None
};

void	*xzoom_init(unsigned w, unsigned h) {
	XSetWindowAttributes	wndattr;
	Colormap				cmap;
	XVisualInfo				*vinf;
	Window					root;
	t_wnd					*wnd;

	wnd = calloc(1, sizeof(t_wnd));
	if (!wnd) {
		return (0);
	}
	wnd->dsp = XOpenDisplay(NULL);
	root = DefaultRootWindow(wnd->dsp);
	vinf = glXChooseVisual(wnd->dsp, 0, vinf_attr);
	cmap = XCreateColormap(wnd->dsp, root, vinf->visual, AllocNone);
	wndattr.colormap = cmap;
	wndattr.event_mask = KeyPress | ClientMessage;
	wnd->id = XCreateWindow(
		wnd->dsp,
		DefaultRootWindow(wnd->dsp),
		0, 0, w, h, 0,
		vinf->depth,
		InputOutput,
		vinf->visual,
		CWColormap | CWEventMask,
		&wndattr
	);
	__xzoom_fullscreen(wnd);
	XMapWindow(wnd->dsp, wnd->id);
	wnd->ctx = glXCreateContext(wnd->dsp, vinf, 0, 1);
	glXMakeCurrent(wnd->dsp, wnd->id, wnd->ctx);
	XSelectInput(wnd->dsp, wnd->id, KeyPressMask | KeyReleaseMask);
	wnd->wm_quit = XInternAtom(wnd->dsp, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(wnd->dsp, wnd->id, &wnd->wm_quit, 1);
	free(vinf);
	return (wnd);
}

int	xzoom_event_poll(void *wnd) {
	XEvent	event;
	t_wnd	*wptr;

	wptr = (t_wnd *) wnd;
	while (XPending(wptr->dsp)) {
		XNextEvent(wptr->dsp, &event);
		switch (event.type) {
			case (KeyPress): {
				wptr->quit = 1;
			} break;
			case (ClientMessage): {
				if (event.xclient.data.l[0] == wptr->wm_quit) {
					wptr->quit = 1;
				}
			}
		}
	}
	return (1);
}

int	xzoom_should_quit(void *wnd) {
	t_wnd	*wptr;

	wptr = (t_wnd*) wnd;
	return (wptr->quit);
}

int	xzoom_quit(void *wnd) {
	t_wnd	*wptr;

	wptr = (t_wnd *) wnd;
	glXDestroyContext(wptr->dsp, wptr->ctx);
	XUnmapWindow(wptr->dsp, wptr->id);
	XDestroyWindow(wptr->dsp, wptr->id);
	XCloseDisplay(wptr->dsp);
	free(wnd);
	return (1);
}

int	xzoom_swap_buffers(void *wnd) {
	t_wnd	*wptr;

	wptr = (t_wnd *) wnd;
	glXSwapBuffers(wptr->dsp, wptr->id);
	return (1);
}

static int	__xzoom_fullscreen(void *wnd) {
	t_wnd	*wptr;
	Atom	wm_state;
	Atom	wm_fullscr;

	wptr = (t_wnd *) wnd;
	wm_state = XInternAtom(wptr->dsp, "_NET_WM_STATE", 1);
	wm_fullscr = XInternAtom(wptr->dsp, "_NET_WM_STATE_FULLSCREEN", 1);
	XChangeProperty(
		wptr->dsp, wptr->id,
		wm_state, XA_ATOM, 32,
        PropModeReplace, 
		(unsigned char *) &wm_fullscr,
		1
	);
	return (1);
}
