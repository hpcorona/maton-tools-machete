#pragma once

#include <string.h>
#include "utils.h"
#include "../math/vector.h"
#include "../platform/platform.h"
#include "../graphics/draw.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <zip.h>
#include <zipint.h>
#include <time.h>
extern "C" {
#include <png.h>
}
#include <stdio.h>

// START OF HACK TO END THE APP
__BEGIN_DECLS
extern __noreturn void exit(int);
__END_DECLS
// END OF HACK TO END THE APP

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

  double AbsoluteTime();

  void LaunchURL(const char * url);

	void Exit();

protected:
  void OpenAPK();
  void CloseAPK();
  png_byte* LoadPNG(const char* filename, int &width, int &height);

private:
  zip* apk;
  png_byte* imageData;
  char* apkName;
  char* dirName;
  char* apkFile;
  machete::data::Iterator<char*> mems;
  machete::data::Hash<machete::data::Str, struct AndroidResource*> resources;
};
