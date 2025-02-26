#include "xzoom.h"
#include "xzoom-opengl.h"

#include <stdlib.h>

static const char	*glsl_vertex = 
"#version 460 core\n"
"layout (location = 0) in vec3  a_Pos;\n"
"layout (location = 1) in vec4  a_Col;\n"
"layout (location = 2) in vec2  a_TexCoord;\n"
"out vec4						v_Col;\n"
"out vec2						v_TexCoord;\n"
"uniform mat4					u_proj;\n"
"uniform mat4					u_view;\n"
"void main() {\n"
" 	gl_Position = u_proj * u_view * vec4(a_Pos, 1.0f);\n"
"	v_Col = a_Col;\n"
"	v_TexCoord = a_TexCoord;\n"
"}";

static const char	*glsl_fragment = 
"#version 460 core\n"
"in vec4			v_Col;\n"
"in vec2			v_TexCoord;\n"
"out vec4			f_Col;\n"
"uniform sampler2D	u_Texture;\n"
"void main() {\n"
"   f_Col = texture(u_Texture, v_TexCoord) * v_Col;\n"
"}";

void	*xzoom_render(void *wnd) {
	unsigned	glsl_vert;
	unsigned	glsl_frag;
	unsigned	index_data[XZOOM_QUADC * 6];
	unsigned	texture_data;
	t_rndr		*rndr;

	rndr = (t_rndr *) calloc(1, sizeof(t_rndr));
	if (!rndr) {
		return (0);
	}
	rndr->wnd = wnd;
	/* Shader creation */
	glsl_vert = glCreateShader(GL_VERTEX_SHADER);
	glsl_frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(glsl_vert, 1, &glsl_vertex, 0);
	glShaderSource(glsl_frag, 1, &glsl_fragment, 0);
	glCompileShader(glsl_vert);
	glCompileShader(glsl_frag);
	rndr->s_gl.shd = glCreateProgram();
	glAttachShader(rndr->s_gl.shd, glsl_vert);
	glAttachShader(rndr->s_gl.shd, glsl_frag);
	glLinkProgram(rndr->s_gl.shd);
	glDeleteShader(glsl_vert);
	glDeleteShader(glsl_frag);
	/* Buffers and arrays creation */
	for (int i = 0, off = 0; i < XZOOM_QUADC * 6; off += 4) {
		index_data[i++] = 0 + off;
		index_data[i++] = 1 + off;
		index_data[i++] = 2 + off;
		index_data[i++] = 1 + off;
		index_data[i++] = 2 + off;
		index_data[i++] = 3 + off;
	}
	glGenVertexArrays(1, &rndr->s_gl.vao);
	glBindVertexArray(rndr->s_gl.vao);
	glGenBuffers(1, &rndr->s_gl.vbo);
	glGenBuffers(1, &rndr->s_gl.ebo);
	glBindBuffer(GL_ARRAY_BUFFER, rndr->s_gl.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rndr->s_gl.ebo);
	glBufferData(GL_ARRAY_BUFFER, XZOOM_QUADC * 9 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 9 * sizeof(float), (void *) 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, 0, 9 * sizeof(float), (void *) 12);
	glVertexAttribPointer(2, 2, GL_FLOAT, 0, 9 * sizeof(float), (void *) 28);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	/* Textures */
	texture_data = 0xffffffff;
	glGenTextures(1, &rndr->s_txt.def);
	glBindTexture(GL_TEXTURE_2D, rndr->s_txt.def);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	rndr->s_txt.cur = rndr->s_txt.def;
	/* Projection setup */
	xzoom_mat4_identity(rndr->s_proj.mat_v);
	xzoom_mat4_ortho(rndr->s_proj.mat_p, 0.0f, xzoom_window_h(wnd), 0.0f, xzoom_window_w(wnd));
	glUseProgram(rndr->s_gl.shd);
	rndr->s_proj.mat_p_loc = glGetUniformLocation(rndr->s_gl.shd, "u_proj");
	rndr->s_proj.mat_v_loc = glGetUniformLocation(rndr->s_gl.shd, "u_view");
	glUseProgram(0);
	return (rndr);
}

int	xzoom_begin(void *rndr) {
	t_rndr	*rptr;

	rptr = (t_rndr *) rndr;
	glUseProgram(rptr->s_gl.shd);
	glUniformMatrix4fv(rptr->s_proj.mat_p_loc, 1, 0, &rptr->s_proj.mat_p[0][0]);
	glUniformMatrix4fv(rptr->s_proj.mat_v_loc, 1, 0, &rptr->s_proj.mat_v[0][0]);
	glUseProgram(0);
	return (1);
}

int	xzoom_end(void *rndr) {
	t_rndr	*rptr;

	rptr = (t_rndr *) rndr;
	glUseProgram(rptr->s_gl.shd);
	glBindVertexArray(rptr->s_gl.vao);
	glBindTextureUnit(0, rptr->s_txt.cur);
	glDrawElements(GL_TRIANGLES, rptr->s_quad.cnt * 6, GL_UNSIGNED_INT, 0);
	glBindTextureUnit(0, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	rptr->s_quad.cnt = 0;
	return (1);
}

int	xzoom_render_quit(void *rndr) {
	t_rndr	*rptr;

	rptr = (t_rndr *) rndr;
	glDeleteProgram(rptr->s_gl.shd);
	glDeleteBuffers(1, &rptr->s_gl.vbo);
	glDeleteBuffers(1, &rptr->s_gl.ebo);
	glDeleteVertexArrays(1, &rptr->s_gl.vao);
	glDeleteTextures(1, &rptr->s_txt.def);
	free(rndr);
	return (1);
}

int		xzoom_default_texture(void *rndr) {
	t_rndr	*rptr;

	rptr = (t_rndr *) rndr;
	return (rptr->s_txt.def);
}
