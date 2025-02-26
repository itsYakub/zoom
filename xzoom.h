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

typedef float	t_vec2[2];
typedef float	t_vec3[3];
typedef float	t_vec4[4];
typedef float	t_mat4x4[4][4];

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
	struct {
		t_mat4x4	mat_p;
		t_mat4x4	mat_v;
		unsigned	mat_p_loc;
		unsigned	mat_v_loc;
	}	s_proj;
	void	*wnd;
}	t_rndr;

/* xzoom-window.c */
void		*xzoom_init(void);
int			xzoom_event_poll(void *);
int			xzoom_should_quit(void *);
int			xzoom_quit(void *);
int			xzoom_swap_buffers(void *);
int			xzoom_window_w(void *);
int			xzoom_window_h(void *);

/* xzoom-render.c */
void		*xzoom_render(void *);
int			xzoom_begin(void *);
int			xzoom_end(void *);
int			xzoom_render_quit(void *);
int			xzoom_default_texture(void *);

/* xzoom-draw.c */
int			xzoom_draw_quad(void *, float [3], float [3], float [3], float [3], float [4], float [4], unsigned);
int			xzoom_draw_texture(void *, float [4], float [4], unsigned);

/* xzoom-opengl.c */
int			xzoom_loadgl(void *(*)(const char *));
unsigned	xzoom_texture(unsigned, unsigned, char *);
int			xzoom_texture_free(unsigned);

/* xzoom-screenshot.c */
char		*xzoom_screenshot(void);

/* xzoom-math.c */
int			xzoom_mat4_identity(t_mat4x4);
int			xzoom_mat4_ortho(t_mat4x4, float, float, float, float);

#endif
