#include "xzoom-opengl.h"
#include "xzoom.h"

/*
 *	TODO:
 *	Load more opengl functions and start building a proper renderer
 * */

PFNGLCREATESHADERPROC		glCreateShader = 0;
PFNGLSHADERSOURCEPROC		glShaderSource = 0;
PFNGLCOMPILESHADERPROC		glCompileShader = 0;
PFNGLDELETESHADERPROC		glDeleteShader = 0;
PFNGLCREATEPROGRAMPROC		glCreateProgram = 0;
PFNGLATTACHSHADERPROC		glAttachShader = 0;
PFNGLLINKPROGRAMPROC		glLinkProgram = 0;
PFNGLDELETEPROGRAMPROC		glDeleteProgram = 0;
PFNGLUSEPROGRAMPROC			glUseProgram = 0;
PFNGLGETUNIFORMLOCATIONPROC	glGetUniformLocation = 0;
PFNGLUNIFORMMATRIX4FVPROC	glUniformMatrix4fv = 0;
PFNGLGENVERTEXARRAYSPROC				glGenVertexArrays = 0;
PFNGLBINDVERTEXARRAYPROC				glBindVertexArray = 0;
PFNGLDELETEVERTEXARRAYSPROC				glDeleteVertexArrays = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC		glEnableVertexAttribArray = 0;
PFNGLDISABLEVERTEXATTRIBARRAYPROC		glDisableVertexAttribArray = 0;
PFNGLVERTEXATTRIBPOINTERPROC			glVertexAttribPointer = 0;
PFNGLGENBUFFERSPROC		glGenBuffers = 0;
PFNGLBINDBUFFERPROC		glBindBuffer = 0;
PFNGLBUFFERDATAPROC		glBufferData = 0;
PFNGLBUFFERSUBDATAPROC	glBufferSubData = 0;
PFNGLDELETEBUFFERSPROC	glDeleteBuffers = 0;
PFNGLCREATETEXTURESPROC		glCreateTextures = 0;
PFNGLBINDTEXTUREUNITPROC	glBindTextureUnit = 0;
PFNGLGENERATEMIPMAPPROC		glGenerateMipmap = 0;

int	xzoom_loadgl(void *(*load)(const char *)) {
	/* Shader manipulation functions */
	glCreateShader = (PFNGLCREATESHADERPROC) load("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC) load("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC) load("glCompileShader");
	glDeleteShader = (PFNGLDELETESHADERPROC) load("glDeleteShader");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC) load("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC) load("glAttachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC) load("glLinkProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)load("glDeleteProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC) load("glUseProgram");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) load("glGetUniformLocation");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) load("glUniformMatrix4fv");

	/* Vertex array manipulation functions*/
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) load("glGenVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) load("glBindVertexArray");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) load("glDeleteVertexArrays");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) load("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) load("glDisableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) load("glVertexAttribPointer");

	/* Buffer manipulation functions */
	glGenBuffers = (PFNGLGENBUFFERSPROC) load("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC) load("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC) load("glBufferData");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC) load("glBufferSubData");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) load("glDeleteBuffers");

	/* Texture manipulation functions */
	glCreateTextures = (PFNGLCREATETEXTURESPROC) load("glCreateTextures");
	glBindTextureUnit = (PFNGLBINDTEXTUREUNITPROC) load("glBindTextureUnit");
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) load("glGenerateMipmap");

	return (1);
}

unsigned	xzoom_texture(unsigned width, unsigned height, char *dat) {
	unsigned	id;

	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		dat
	);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return (id);
}

int	xzoom_texture_free(unsigned id) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &id);
	return (1);
}
