#pragma once

#include <string.h>
#include "../math/vector.h"
#include "../platform/platform.h"
#include "../graphics/draw.h"
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>

#define LOGE printf
extern "C" void create_writable_file(const char*);

class EmscriptenPlatform : public machete::IPlatform {
public:
  EmscriptenPlatform();
  ~EmscriptenPlatform();

  const char* GetResourcePath() const;

  void LoadImage(const char* filename, void **data, machete::math::IVec2 & size);
	
  void UnloadImage();

  unsigned int LoadFile(const char* name, char** data);

  unsigned int LoadAudio(const char* name);

  unsigned int Random();

  FILE* OpenFile(const char* name, unsigned long &size);

  void CloseFile(FILE* handle);

  char* WritableFile(const char* name);

  double AbsoluteTime();

  void LaunchURL(const char * url);

	void Exit();

private:
	
  SDL_Surface* imageData;

};
