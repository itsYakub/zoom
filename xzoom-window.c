#include "xzoom.h"

#include <X11/X.h>
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
static int	__xzoom_query_mousepos(void *);
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

void	*xzoom_init(void) {
	XSetWindowAttributes	wndattr0;
	XWindowAttributes		wndattr1;
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
	XGetWindowAttributes(wnd->dsp, DefaultRootWindow(wnd->dsp), &wndattr1);
	wndattr0.colormap = cmap;
	wndattr0.background_pixmap = None;
    wndattr0.border_pixel = 0;
	wndattr0.event_mask = CWColormap | CWBorderPixel | CWBackPixel | CWEventMask | MotionNotify | KeyPress | ButtonPress | ButtonRelease | ClientMessage;
	/* Creating an X11 window */
	wnd->id = XCreateWindow(
		wnd->dsp,
		DefaultRootWindow(wnd->dsp),
		0, 0,
		wndattr1.width, wndattr1.height, 0,
		vinf->depth,
		InputOutput,
		vinf->visual,
		CWColormap | CWBorderPixel | CWBackPixel | CWEventMask,
		&wndattr0
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
	XSelectInput(wnd->dsp, wnd->id, KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
	__xzoom_fullscreen(wnd);
	__xzoom_query_mousepos(wnd); 
	xzoom_loadgl((void *(*)(const char *)) glXGetProcAddress);
	glViewport(0, 0, wndattr1.width, wndattr1.height);
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
	wptr->s_input.ptr_pos_prev[0] = wptr->s_input.ptr_pos[0];
	wptr->s_input.ptr_pos_prev[1] = wptr->s_input.ptr_pos[1];
	while (XPending(wptr->dsp)) {
		XNextEvent(wptr->dsp, &event);
		switch (event.type) {
			case (MotionNotify): {
				wptr->s_input.ptr_pos[0] = (float) event.xmotion.x;
				wptr->s_input.ptr_pos[1] = (float) event.xmotion.y;
			} break;
			case (ButtonPress): {
				if (event.xbutton.button == 4) {
					wptr->s_input.ptr_wheel = 0.8f;
				}
				else if (event.xbutton.button == 5) {
					wptr->s_input.ptr_wheel = -0.8f;
				}
				else {
					wptr->s_input.ptr_prs = 1;
				}
			} break;
			case (ButtonRelease): {
				wptr->s_input.ptr_prs = 0;
			} break;
			case (KeyPress): {
				wptr->quit = 1;
			} break;
			case (ClientMessage): {
				if (event.xclient.data.l[0] == (long int) wptr->wm_quit) {
					wptr->quit = 1;
				}
			} break;
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

int	xzoom_window_w(void *wnd) {
	XWindowAttributes	attr;
	t_wnd				*wptr;

	wptr = (t_wnd *) wnd;
	XGetWindowAttributes(wptr->dsp, wptr->id, &attr);
	return (attr.width);
}

int	xzoom_window_h(void *wnd) {
	XWindowAttributes	attr;
	t_wnd				*wptr;

	wptr = (t_wnd *) wnd;
	XGetWindowAttributes(wptr->dsp, wptr->id, &attr);
	return (attr.height);
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

/* 
 * At the beginning, mouse position is set to (0,0), 
 * thus we need to set it to something to not scroll to the top-left corner at the beginning 
 * (before the mouse motion event) 
 * */
static int	__xzoom_query_mousepos(void *wnd) {
	Window		wnd_return, wnd_child;
	unsigned	result;
	int			root_x, root_y;
	int			win_x, win_y;
	t_wnd		*wptr;

	wptr = (t_wnd *) wnd;
	XQueryPointer(
		wptr->dsp,
		wptr->id,
		&wnd_return,
		&wnd_child,
		&root_x, &root_y,
		&win_x, &win_y,
		&result
	);
	wptr->s_input.ptr_pos[0] = (float) win_x;
	wptr->s_input.ptr_pos[1] = (float) win_y;
	return (1);
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
