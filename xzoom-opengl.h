#ifndef XZOOM_OPENGL_H
# define XZOOM_OPENGL_H

# include <GL/gl.h>
# include <GL/glext.h>

extern PFNGLCREATESHADERPROC		glCreateShader;
extern PFNGLSHADERSOURCEPROC		glShaderSource;
extern PFNGLCOMPILESHADERPROC		glCompileShader;
extern PFNGLDELETESHADERPROC		glDeleteShader;
extern PFNGLCREATEPROGRAMPROC		glCreateProgram;
extern PFNGLATTACHSHADERPROC		glAttachShader;
extern PFNGLLINKPROGRAMPROC			glLinkProgram;
extern PFNGLDELETEPROGRAMPROC		glDeleteProgram;
extern PFNGLUSEPROGRAMPROC			glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC	glGetUniformLocation;
extern PFNGLUNIFORMMATRIX4FVPROC	glUniformMatrix4fv;
extern PFNGLGENVERTEXARRAYSPROC				glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC				glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC			glDeleteVertexArrays;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC		glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC	glDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC			glVertexAttribPointer;
extern PFNGLGENBUFFERSPROC		glGenBuffers;
extern PFNGLBINDBUFFERPROC		glBindBuffer;
extern PFNGLBUFFERDATAPROC		glBufferData;
extern PFNGLBUFFERSUBDATAPROC	glBufferSubData;
extern PFNGLDELETEBUFFERSPROC	glDeleteBuffers;
extern PFNGLCREATETEXTURESPROC		glCreateTextures;
extern PFNGLBINDTEXTUREUNITPROC		glBindTextureUnit;
extern PFNGLGENERATEMIPMAPPROC		glGenerateMipmap;

#endif
