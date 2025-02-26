#ifndef XZOOM_H
# define XZOOM_H

# ifndef XZOOM_RESW
#  define XZOOM_RESW 1920
# endif
# ifndef XZOOM_RESH
#  define XZOOM_RESH 1080
# endif
# ifndef XZOOM_QUADC
#  define XZOOM_QUADC 128
# endif

typedef struct s_wnd {
	void			*dsp;
	void			*ctx;
	unsigned		id;
	unsigned		quit;
	unsigned long	wm_quit;
}	t_wnd;

typedef struct s_rndr {
	struct {
		unsigned	vao;
		unsigned	vbo;
		unsigned	ebo;
		unsigned	shd;
	}	s_gl;
	struct {
		unsigned	cnt;
	}	s_quad;
	struct {
		unsigned	def;
		unsigned	cur;
	}	s_txt;
	void	*wnd;
}	t_rndr;

/* xzoom-window.c */
void	*xzoom_init(unsigned, unsigned);
int		xzoom_event_poll(void *);
int		xzoom_should_quit(void *);
int		xzoom_quit(void *);
int		xzoom_swap_buffers(void *);

/* xzoom-render.c */
void	*xzoom_render(void *);
int		xzoom_begin(void *);
int		xzoom_end(void *);
int		xzoom_render_quit(void *);
int		xzoom_default_texture(void *);

/* xzoom-draw.c */
int		xzoom_draw_quad(void *, float [3], float [3], float [3], float [3], float [4], float [4], unsigned);

/* xzoom-opengl.c */
int		xzoom_loadgl(void *(*)(const char *));

#endif
