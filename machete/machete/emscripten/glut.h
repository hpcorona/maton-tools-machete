//#define GL_GLEXT_PROTOTYPES
//#define EGL_EGLEXT_PROTOTYPES
#define GL_VERSION_2_0
#define GL_VERSION_1_5

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
extern "C" {
#include <GL/glut.h>
}

void InitializePlatform(int width, int height, const char *title, void (*func)(float), void (*fund2)());
void LaunchGameLoop();
