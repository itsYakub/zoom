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

typedef struct s_cam {
	t_vec2	pos;
	t_vec2	off;
	float	scl;
}	t_cam;

typedef struct s_wnd {
	void			*dsp;
	void			*ctx;
	unsigned		id;
	unsigned		quit;
	unsigned long	wm_quit;
	struct {
		int		ptr_prs;
		t_vec2	ptr_wheel;
		t_vec2	ptr_pos_prev;
		t_vec2	ptr_pos;
	}	s_input;
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
		t_cam		cam_def;
		t_cam		cam_cur;
		t_mat4x4	mat_p;
		t_mat4x4	mat_v;
		unsigned	mat_p_loc;
		unsigned	mat_v_loc;
	}	s_proj;
	void	*wnd;
}	t_rndr;

/* xzoom.c */
int			xzoom_pan(void *, t_cam *);
int			xzoom_zoom(void *, t_cam *);

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
int			xzoom_begin_camera(void *, t_cam);
int			xzoom_end_camera(void *);
int			xzoom_render_quit(void *);
int			xzoom_default_texture(void *);

/* xzoom-input.c */
int			xzoom_mouse_pressed(void *);
float		xzoom_mouse_wheel(void *);
float		xzoom_mouse_x(void *);
float		xzoom_mouse_y(void *);
float		xzoom_mouse_prev_x(void *);
float		xzoom_mouse_prev_y(void *);
int			xzoom_screen_to_world(t_cam *, t_vec2, t_vec2);

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
int			xzoom_mat4_inverse(t_mat4x4);
int			xzoom_mat4_ortho(t_mat4x4, float, float, float, float);
int			xzoom_mat4_trans(t_mat4x4, t_vec3, t_mat4x4);
int			xzoom_mat4_scale(t_mat4x4, t_vec3, t_mat4x4);
int			xzoom_mat4_mulv(t_mat4x4, t_vec4, t_vec4);
int			xzoom_vec2_scale(t_vec2, float, t_vec2);
int			xzoom_vec4_scale(t_vec4, float, t_vec4);
int			xzoom_vec4_muladds(t_vec4, float, t_vec4);

#endif
