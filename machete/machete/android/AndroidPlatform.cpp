#include "AndroidPlatform.h"

__BEGIN_DECLS
extern unsigned int arc4random(void);
__END_DECLS

machete::graphics::DrawContext* CreateDrawContext(machete::graphics::RenderTarget target, int width, int height) {

  machete::graphics::DrawContext *ctx = new machete::graphics::DrawContext(target);

  ctx->Initialize(width, height);

  return ctx;
}

AndroidPlatform::AndroidPlatform(const char* APKfile) {
	LOGI("Initializing Android Platform with APK %s", APKfile);
	APKArchive = loadAPK(APKfile);
  apkName = new char[50];
  apkName[0] = 0;
  strcat(apkName, APKfile);
  strcat(apkName, "/assets/");

  imageData = NULL;
}

AndroidPlatform::~AndroidPlatform() {
	zip_close(APKArchive);
  delete apkName;
}

const char* AndroidPlatform::GetResourcePath() const {
  return (const char*)apkName;
}

void AndroidPlatform::LoadImage(const char* filename, void **data, machete::math::IVec2 & size) {
  UnloadImage();

  char pngfile[100];
	pngfile[0] = 0;
	strcat(pngfile, "assets/");
	strcat(pngfile, filename);

	LOGI("open %s", filename);
	imageData = loadTextureFromPNG(APKArchive, pngfile, size.x, size.y);

  *data = (void*)imageData;
}

void AndroidPlatform::UnloadImage() {
	delete[] imageData;
	imageData = NULL;
}

unsigned int AndroidPlatform::LoadFile(const char* name, char** data) {
  char fname[100];
  fname[0] = 0;
  strcat(fname, "assets/");
  strcat(fname, name);

  return loadFile(name, data);
}

unsigned int AndroidPlatform::LoadAudio(const char* name) {
}

inline unsigned int AndroidPlatform::Random() {
  return arc4random();
}

FILE* AndroidPlatform::OpenFile(const char* name) {
}

void AndroidPlatform::CloseFile(FILE* handle) {
  fclose(handle);
}

char* AndroidPlatform::WritableFile(const char* name) {
  char* path = new char[100];
  path[0] = 0;

  strcat(path, "/sdcard/matongames/");
  strcat(path, apkName);
  strcat(path, name);

  return path;
}

