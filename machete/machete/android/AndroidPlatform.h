#pragma once

#include <string.h>
#include "utils.h"
#include "../math/vector.h"
#include "../platform/platform.h"
#include "../graphics/draw.h"

struct AndroidResource {
  long offset;
  long length;
};

class AndroidPlatform : public machete::IPlatform {
public:
  AndroidPlatform(const char* APKfile, const char* name);
  ~AndroidPlatform();

  const char* GetResourcePath() const;

  void LoadImage(const char* filename, void **data, machete::math::IVec2 & size);
  void UnloadImage();

  unsigned int LoadFile(const char* name, char** data);

  unsigned int LoadAudio(const char* name);

  unsigned int Random();

  FILE* OpenFile(const char* name, unsigned long &size);

  void CloseFile(FILE* handle);

  char* WritableFile(const char* name);

  void SetResourceData(const char* name, long offset, long length);

private:
    zip* APKArchive;
    png_byte* imageData;
    char* apkName;
    char* dirName;
    char* apkFile;
    machete::data::Iterator<char*> mems;
    machete::data::Hash<machete::data::Str, struct AndroidResource*> resources;
};

