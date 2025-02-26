#include "xzoom.h"
#include "xzoom-opengl.h"

int	xzoom_draw_quad(void *rndr, float v0[3], float v1[3], float v2[3], float v3[3], float col[4], float uv[4], unsigned id) {
	t_rndr	*rptr;
	float	data[] = {
		v0[0], v0[1], v0[2], col[0], col[1], col[2], col[3], uv[0], uv[1],
		v1[0], v1[1], v1[2], col[0], col[1], col[2], col[3], uv[2], uv[1],
		v2[0], v2[1], v2[2], col[0], col[1], col[2], col[3], uv[0], uv[3],
		v3[0], v3[1], v3[2], col[0], col[1], col[2], col[3], uv[2], uv[3],
	};

	rptr = (t_rndr *) rndr;
	if (rptr->s_quad.cnt >= XZOOM_QUADC || rptr->s_txt.cur != id) {
		xzoom_end(rndr);
		rptr->s_txt.cur = id;
		xzoom_begin(rndr);
	}
	glBindVertexArray(rptr->s_gl.vao);
	glBindBuffer(GL_ARRAY_BUFFER, rptr->s_gl.vbo);
	glBufferSubData(GL_ARRAY_BUFFER, rptr->s_quad.cnt++ * sizeof(data), sizeof(data), data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return (1);
}
