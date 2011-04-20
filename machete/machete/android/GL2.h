
#ifndef GL2_H_INCLUDED
#define GL2_H_INCLUDED

#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>
#include <dlfcn.h>

extern bool OpenGLES2Init();
extern void OpenGLES2Dealloc();

#ifdef  __cplusplus
extern "C" {
#endif

#define IMPORTGL_API
#define IMPORTGL_FNPTRINIT = NULL
#define FNDEF(retType, funcName, args) typedef IMPORTGL_API retType (*funcPtrT_##funcName) args; funcPtrT_##funcName funcPtr_##funcName IMPORTGL_FNPTRINIT

FNDEF(void, glActiveTexture, (GLenum texture));
FNDEF(void, glAttachShader, (GLuint program, GLuint shader));
FNDEF(void, glBindBuffer, (GLenum target, GLuint buffer));
FNDEF(void, glBindTexture, (GLenum target, GLuint texture));
FNDEF(void, glBlendFunc, (GLenum sfactor, GLenum dfactor));
FNDEF(void, glBufferData, (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage));
FNDEF(void, glBufferSubData, (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data));
FNDEF(void, glClear, (GLbitfield mask));
FNDEF(void, glClearColor, (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha));
FNDEF(void, glCompileShader, (GLuint shader));
FNDEF(GLuint, glCreateProgram, (void));
FNDEF(GLuint, glCreateShader, (GLenum shaderType));
FNDEF(void, glDeleteBuffers, (GLsizei n, const GLuint* buffers));
FNDEF(void, glDisable, (GLenum cap));
FNDEF(void, glDrawElements, (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices));
FNDEF(void, glEnable, (GLenum cap));
FNDEF(void, glEnableVertexAttribArray, (GLuint index));
FNDEF(void, glGenBuffers, (GLsizei n, GLuint* buffers));
FNDEF(void, glGenTextures, (GLsizei n, GLuint* textures));
FNDEF(GLint, glGetAttribLocation, (GLuint program, const char* name));
FNDEF(GLint, glGetUniformLocation, (GLuint program, const char* name));
FNDEF(void, glLinkProgram, (GLuint program));
FNDEF(void, glShaderSource, (GLuint shader, GLsizei count, const char** string, const GLint* length));
FNDEF(void, glTexImage2D, (GLenum target, GLint level, GLint iformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data));
FNDEF(void, glUseProgram, (GLuint program));
FNDEF(void, glVertexAttribPointer, (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer));
FNDEF(void, glViewport, (GLint x, GLint y, GLsizei width, GLsizei height));
FNDEF(void, glUniform1f, (GLint location, GLfloat x));
FNDEF(void, glUniformMatrix4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value));
FNDEF(void, glTexParameteri, (GLenum target, GLenum pname, GLint param));
FNDEF(void, glGetProgramiv, (GLuint program, GLenum pname, GLint* params));
FNDEF(void, glGetShaderiv, (GLuint shader, GLenum pname, GLint* params));

#undef FNDEF
#define FNPTR(name) funcPtr_##name

#define glActiveTexture				FNPTR(glActiveTexture)
#define glAttachShader					FNPTR(glAttachShader)
#define glBindBuffer					FNPTR(glBindBuffer)
#define glBindTexture					FNPTR(glBindTexture)
#define glBlendFunc					FNPTR(glBlendFunc)
#define glBufferData					FNPTR(glBufferData)
#define glBufferSubData				FNPTR(glBufferSubData)
#define glClear						FNPTR(glClear)
#define glClearColor					FNPTR(glClearColor)
#define glCompileShader				FNPTR(glCompileShader)
#define glCreateProgram				FNPTR(glCreateProgram)
#define glCreateShader					FNPTR(glCreateShader)
#define glDeleteBuffers				FNPTR(glDeleteBuffers)
#define glDisable						FNPTR(glDisable)
#define glDrawElements					FNPTR(glDrawElements)
#define glEnable						FNPTR(glEnable)
#define glEnableVertexAttribArray		FNPTR(glEnableVertexAttribArray)
#define glGenBuffers					FNPTR(glGenBuffers)
#define glGenTextures					FNPTR(glGenTextures)
#define glGetAttribLocation			FNPTR(glGetAttribLocation)
#define glGetUniformLocation			FNPTR(glGetUniformLocation)
#define glLinkProgram					FNPTR(glLinkProgram)
#define glShaderSource					FNPTR(glShaderSource)
#define glTexImage2D					FNPTR(glTexImage2D)
#define glUseProgram					FNPTR(glUseProgram)
#define glVertexAttribPointer			FNPTR(glVertexAttribPointer)
#define glViewport						FNPTR(glViewport)
#define glUniform1f					FNPTR(glUniform1f)
#define glUniformMatrix4fv				FNPTR(glUniformMatrix4fv)
#define glTexParameteri				FNPTR(glTexParameteri)
#define glGetProgramiv					FNPTR(glGetProgramiv)
#define glGetShaderiv					FNPTR(glGetShaderiv)

#endif

#ifdef  __cplusplus
}
#endif
