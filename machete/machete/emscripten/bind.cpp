
#include "emscripten.h"
#include "glut.h"
#include "../engine.h"
#include <stdio.h>

EmscriptenPlatform* platform;
static machete::IGame* game;
machete::graphics::DrawContext* renEngine;
int width, height;
bool initialized = false;

machete::graphics::DrawContext* CreateDrawContext(machete::graphics::RenderTarget target, int width, int height) {
	machete::graphics::DrawContext *renEngine = new machete::graphics::DrawContext(target);

  renEngine->Initialize(width, height);

  return renEngine;
}

void DrawGame() {
	if (game == NULL) return;
	game->Render();
}

void UpdateGame(float time) {
	if (game == NULL) return;
	game->Update(time);
}

void InitializeGame(int w, int h) {
	InitializePlatform(w, h, "Maton Games", UpdateGame, DrawGame);
	
	platform = new EmscriptenPlatform();

  machete::Start(platform);
	
	machete::StartGraphics();

  renEngine = CreateDrawContext(machete::graphics::TargetScreen, w, h);

  width = w;
  height = h;
}

void RunGame() {
	game = CreateGame();
	game->Initialize(renEngine, width, height, machete::DeviceOrientationPortrait);
	game->OnStart();
	game->OnResume();
	
	LaunchGameLoop();
}

int main(int argc, char *argv[]) {
	InitializeGame(320, 480);
	RunGame();
}
