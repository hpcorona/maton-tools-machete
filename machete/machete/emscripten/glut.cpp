#include "glut.h"
#include <sys/time.h>
#include "../input/default.h"
#include "../common/log.h"

static void (*game_update_func)(float);
static void (*game_render_func)();
static struct timeval t1, t2;
static struct timezone tz;
static float deltatime;
static bool mouseDown = false;

void GlutMotionFunc(int x, int y) {
	if (mouseDown == false) return;
	
	machete::input::SingleTouchHandler(0, 1, machete::input::TouchMove, x, y);
}

void GlutMouseFunc(int button, int state, int x, int y) {
	if (button == 0) {
		if (state == 1 && mouseDown == true) {
			mouseDown = false;
			machete::input::SingleTouchHandler(0, 1, machete::input::TouchEnd, x, y);
		} else {
			mouseDown = true;
			machete::input::SingleTouchHandler(0, 1, machete::input::TouchStart, x, y);
		}
	}
}

void GameDraw() {
	// All is done in the GameLoop
}

void GameLoop() {
  gettimeofday(&t2, &tz);
  deltatime = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
  t1 = t2;
	
	game_update_func(deltatime);
	game_render_func();
	glutSwapBuffers();
}

void InitializePlatform(int width, int height, const char *title, void (*func)(float), void (*func2)()) {
	glutInit(0, 0);
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow(title);
	glutMouseFunc(GlutMouseFunc);
	glutMotionFunc(GlutMotionFunc);
	glutDisplayFunc(GameDraw);
	glutIdleFunc(GameLoop);
	game_render_func = func2;
	game_update_func = func;
}

void LaunchGameLoop() {
  gettimeofday(&t1 , &tz);
	glutMainLoop();
}
