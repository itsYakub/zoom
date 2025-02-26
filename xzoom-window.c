#include "xzoom.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>

typedef GLXContext (* PFNGLXCREATECONTEXTATTRIBSARBPROC) (Display *, GLXFBConfig, GLXContext, Bool, const int *);
PFNGLXCREATECONTEXTATTRIBSARBPROC	glXCreateContextAttribsARB;

static void	*__xzoom_fbconfig(void *, int *);
static int	__xzoom_fullscreen(void *);

static int	glattr[] = {
	GLX_USE_GL,				1,
	GLX_DOUBLEBUFFER,		1,
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
	GLXFBConfig				*fbconf;
	XVisualInfo				*vinf;
	int						fbconf_best;
	int						context_attr[7];
	t_wnd					*wnd;

	/* Allocating window object */
	wnd = calloc(1, sizeof(t_wnd));
	if (!wnd) {
		return (0);
	}
	wnd->dsp = XOpenDisplay(NULL);
	/* Getting the best framebuffer configuration and initializing visualinfo with them */
	fbconf = __xzoom_fbconfig(wnd, &fbconf_best);
	vinf = glXGetVisualFromFBConfig(wnd->dsp, fbconf[fbconf_best]);
	/* Creating the colormap */	
	cmap = XCreateColormap(wnd->dsp, DefaultRootWindow(wnd->dsp), vinf->visual, AllocNone);
	/* Setting up window attributes */
	wndattr.colormap = cmap;
	wndattr.background_pixmap = None;
    wndattr.border_pixel = 0;
	wndattr.event_mask = CWColormap | CWBorderPixel | CWBackPixel | CWEventMask | KeyPress | ClientMessage;
	/* Creating an X11 window */
	wnd->id = XCreateWindow(
		wnd->dsp,
		DefaultRootWindow(wnd->dsp),
		0, 0, w, h, 0,
		vinf->depth,
		InputOutput,
		vinf->visual,
		CWColormap | CWBorderPixel | CWBackPixel | CWEventMask,
		&wndattr
	);
	/* Creating a glX context */
	memset(context_attr, 0, sizeof(context_attr));
	context_attr[0]	= GLX_CONTEXT_PROFILE_MASK_ARB;
	context_attr[1] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
	context_attr[2] = GLX_CONTEXT_MAJOR_VERSION_ARB;
	context_attr[3] = 3;
	context_attr[4] = GLX_CONTEXT_MINOR_VERSION_ARB;
	context_attr[5] = 3;
	glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress((GLubyte *) "glXCreateContextAttribsARB");
	wnd->ctx = glXCreateContextAttribsARB(wnd->dsp, fbconf[fbconf_best], 0, 1, context_attr);
	glXMakeCurrent(wnd->dsp, wnd->id, wnd->ctx);
	/* Setting up last things for a window*/
	wnd->wm_quit = XInternAtom(wnd->dsp, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(wnd->dsp, wnd->id, &wnd->wm_quit, 1);
	XSelectInput(wnd->dsp, wnd->id, KeyPressMask | KeyReleaseMask);
	__xzoom_fullscreen(wnd);
	xzoom_loadgl((void *(*)(const char *)) glXGetProcAddress);
	glViewport(0, 0, w, h);
	/* Mapping an X11 window onto the display */
	XMapWindow(wnd->dsp, wnd->id);
	/* Freeing resources */
	XFree(vinf);
	XFree(fbconf);
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
				if (event.xclient.data.l[0] == (long int) wptr->wm_quit) {
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

static void	*__xzoom_fbconfig(void *wnd, int *best) {
	GLXFBConfig	*fbconf;
	XVisualInfo	*vinf;
	unsigned	fbconf_cnt;
	int			sample_buffers;
	int			samples;
	t_wnd		*wptr;

	wptr = (t_wnd *) wnd;
	*best = -1;
	fbconf = glXChooseFBConfig(wptr->dsp, DefaultScreen(wptr->dsp), glattr, (int *) &fbconf_cnt);
	if (!fbconf_cnt) {
		return (0);
	}
	for (int i = 0; i < (int) fbconf_cnt; i++) {
		vinf = glXGetVisualFromFBConfig(wptr->dsp, fbconf[i]);
		if (!vinf) {
			continue;
		}
		XFree(vinf);
		glXGetFBConfigAttrib(wptr->dsp, fbconf[i], GLX_SAMPLE_BUFFERS, &sample_buffers);
		glXGetFBConfigAttrib(wptr->dsp, fbconf[i], GLX_SAMPLES, &samples);
		if ((*best < 0 || sample_buffers) && (!samples && *best == -1)) {
			*best = i;
		}
	}
	if (*best == -1) {
		return (0);
	}
	return (fbconf);
}

static int	__xzoom_fullscreen(void *wnd) {
	Atom	wm_state;
	Atom	wm_fullscr;
	t_wnd	*wptr;

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
