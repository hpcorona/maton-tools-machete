#pragma once

#include <string.h>
#include "utils.h"
#include "../math/vector.h"
#include "../platform/platform.h"
#include "../graphics/draw.h"

class AndroidPlatform : public machete::IPlatform {
public:
  AndroidPlatform(const char* APKfile);
  ~AndroidPlatform();

  const char* GetResourcePath() const;

  void LoadImage(const char* filename, void **data, machete::math::IVec2 & size);
  void UnloadImage();

  unsigned int LoadFile(const char* name, char** data);

  unsigned int LoadAudio(const char* name);

  unsigned int Random();

  FILE* OpenFile(const char* name);

  void CloseFile(FILE* handle);

  char* WritableFile(const char* name);

private:
    zip* APKArchive;
    png_byte* imageData;
    char* apkName;
};

