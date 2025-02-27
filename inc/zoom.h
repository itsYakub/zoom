#ifndef ZOOM_H
# define ZOOM_H

# ifndef ZOOM_QUADC
#  define ZOOM_QUADC 128
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
		float	ptr_wheel;
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

/* zoom.c */
int			zoom_pan(void *, t_cam *);
int			zoom_zoom(void *, t_cam *);

/* zoom-window.c */
void		*zoom_init(void);
int			zoom_event_poll(void *);
int			zoom_should_quit(void *);
int			zoom_quit(void *);
int			zoom_swap_buffers(void *);
int			zoom_window_w(void *);
int			zoom_window_h(void *);

/* zoom-render.c */
void		*zoom_render(void *);
int			zoom_begin(void *);
int			zoom_end(void *);
int			zoom_begin_camera(void *, t_cam);
int			zoom_end_camera(void *);
int			zoom_render_quit(void *);
int			zoom_default_texture(void *);

/* zoom-input.c */
int			zoom_mouse_pressed(void *);
float		zoom_mouse_wheel(void *);
float		zoom_mouse_x(void *);
float		zoom_mouse_y(void *);
float		zoom_mouse_prev_x(void *);
float		zoom_mouse_prev_y(void *);
int			zoom_screen_to_world(t_cam *, t_vec2, t_vec2);

/* zoom-draw.c */
int			zoom_draw_quad(void *, float [3], float [3], float [3], float [3], float [4], float [4], unsigned);
int			zoom_draw_texture(void *, float [4], float [4], unsigned);

/* zoom-opengl.c */
int			zoom_loadgl(void *(*)(const char *));
unsigned	zoom_texture(unsigned, unsigned, char *);
int			zoom_texture_free(unsigned);

/* zoom-screenshot.c */
char		*zoom_screenshot(void);

/* zoom-math.c */
int			zoom_mat4_identity(t_mat4x4);
int			zoom_mat4_inverse(t_mat4x4);
int			zoom_mat4_ortho(t_mat4x4, float, float, float, float);
int			zoom_mat4_trans(t_mat4x4, t_vec3, t_mat4x4);
int			zoom_mat4_scale(t_mat4x4, t_vec3, t_mat4x4);
int			zoom_mat4_mulv(t_mat4x4, t_vec4, t_vec4);
int			zoom_vec2_scale(t_vec2, float, t_vec2);
int			zoom_vec4_scale(t_vec4, float, t_vec4);
int			zoom_vec4_muladds(t_vec4, float, t_vec4);

#endif
