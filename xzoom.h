#ifndef XZOOM_H
# define XZOOM_H

# ifndef XZOOM_RESW
#  define XZOOM_RESW 1920
# endif
# ifndef XZOOM_RESH
#  define XZOOM_RESHi 1080
# endif

/* xzoom-window.c */
void	*xzoom_init(unsigned, unsigned);
int		xzoom_event_poll(void *);
int		xzoom_should_quit(void *);
int		xzoom_quit(void *);
int		xzoom_swap_buffers(void *);

/* xzoom-opengl.c */
int		xzoom_loadgl(void (*)(const char *));

#endif
