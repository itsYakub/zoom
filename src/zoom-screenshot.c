#include "zoom.h"

#include <stdlib.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>

char	*zoom_screenshot(void) {
	XWindowAttributes	attr;
	XImage				*img;
	Display				*dsp;
	char				*data;

	dsp = XOpenDisplay(0);
	if (!dsp) {
		return (0);
	}
	XGetWindowAttributes(dsp, DefaultRootWindow(dsp), &attr);
	img = XGetImage(
		dsp,
		DefaultRootWindow(dsp),
		0, 0,
		attr.width, attr.height,
		AllPlanes,
		ZPixmap
	);
	if (!img) {
		XCloseDisplay(dsp);
		return (0);
	}
	data = calloc(attr.width * attr.height * 4, sizeof(char));
	if (!data) {
		XDestroyImage(img);
		XCloseDisplay(dsp);
		return (0);
	}
	for (int i = 0; i < attr.width * attr.height * 4; i += 4) {
		data[i + 0] = img->data[i + 2];
		data[i + 1] = img->data[i + 1];
		data[i + 2] = img->data[i + 0];
		data[i + 3] = img->data[i + 3];
	}
	XDestroyImage(img);
	XCloseDisplay(dsp);
	return (data);
}
