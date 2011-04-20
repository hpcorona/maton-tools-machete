
#include "GL2.h"

static void *sGLESSO = NULL;

#define IMPORT_FUNC(funcName) procAddress = (void *)dlsym(sGLESSO, #funcName); \
        if (procAddress == NULL) result = 0; \
        funcPtr_##funcName = (funcPtrT_##funcName)(procAddress);

bool OpenGLES2Init() {
	//opengl_es_2_handle = dlopen("/system/lib/egl/libGLESv2.so", RTLD_NOW | RTLD_LOCAL);
    sGLESSO = dlopen("libGLESv2.so", RTLD_NOW);
    if (sGLESSO == NULL) {
        return 0;   // Cannot find OpenGL ES 2.0
    }

    bool result = 1;
    void *procAddress;

    IMPORT_FUNC(glActiveTexture);
    IMPORT_FUNC(glAttachShader);
    IMPORT_FUNC(glBindBuffer);
    IMPORT_FUNC(glBindTexture);
    IMPORT_FUNC(glBlendFunc);
    IMPORT_FUNC(glBufferData);
    IMPORT_FUNC(glBufferSubData);
    IMPORT_FUNC(glClear);
    IMPORT_FUNC(glClearColor);
    IMPORT_FUNC(glCompileShader);
    IMPORT_FUNC(glCreateProgram);
    IMPORT_FUNC(glCreateShader);
    IMPORT_FUNC(glDeleteBuffers);
    IMPORT_FUNC(glDisable);
    IMPORT_FUNC(glDrawElements);
    IMPORT_FUNC(glEnable);
    IMPORT_FUNC(glEnableVertexAttribArray);
    IMPORT_FUNC(glGenBuffers);
    IMPORT_FUNC(glGenTextures);
    IMPORT_FUNC(glGetAttribLocation);
    IMPORT_FUNC(glGetProgramiv);
    IMPORT_FUNC(glGetShaderiv);
    IMPORT_FUNC(glGetUniformLocation);
    IMPORT_FUNC(glLinkProgram);
    IMPORT_FUNC(glShaderSource);
    IMPORT_FUNC(glTexImage2D);
    IMPORT_FUNC(glTexParameteri);
    IMPORT_FUNC(glUniform1f);
    IMPORT_FUNC(glUniformMatrix4fv);
    IMPORT_FUNC(glUseProgram);
    IMPORT_FUNC(glVertexAttribPointer);
    IMPORT_FUNC(glViewport);

    return result;
}

//void OpenGLES2Dealloc() {
//	if (sGLESSO != NULL) {
//		dlclose(sGLESSO);
//	}
//}
