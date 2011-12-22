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
  LOGI("LoadImage");
  LOGI(filename);

  char pngfile[100];
	pngfile[0] = 0;
	strcat(pngfile, "assets/");
	strcat(pngfile, filename);

	LOGI("open %s", filename);
	imageData = loadTextureFromPNG(APKArchive, pngfile, size.x, size.y);

  *data = (void*)imageData;
}

void AndroidPlatform::UnloadImage() {
  if (imageData == NULL) return;

  LOGI("UnloadImage");

	delete[] imageData;
	imageData = NULL;
}

unsigned int AndroidPlatform::LoadFile(const char* name, char** data) {
  int len = strlen(name);
  char* mname = new char[len+1];
  strcpy(mname, name);
  char* ext = mname + len - 4;

  LOGI("LoadFile");
  if (len > 4) {
    LOGI(ext);
    if (strcmp(".xml", ext) == 0 || strcmp(".pos", ext) == 0 || strcmp(".fnt", ext) == 0) {
      LOGI("REPLACED! %s", ext);
      strcpy(ext, ".mbd");
    }
  }

  LOGI(name);
  LOGI(mname);
  char fname[100];
  fname[0] = 0;
  strcat(fname, "assets/");
  strcat(fname, mname);
  delete mname;
  LOGI(fname);

  unsigned int total = loadFile(APKArchive, fname, (void**)data);

  LOGI("Loaded: %d", total);

  return total;
}

unsigned int AndroidPlatform::LoadAudio(const char* name) {
  LOGI("LoadAudio UNIMPLEMENTED");

  return 0;
}

inline unsigned int AndroidPlatform::Random() {
  LOGI("Random()");
  return arc4random();
}

FILE* AndroidPlatform::OpenFile(const char* name) {
  LOGI("OpenFile");
  LOGI(name);
  char fname[100];
  fname[0] = 0;
  strcat(fname, "/sdcard/matongames/assets/");
  strcat(fname, name);
  LOGI(fname);
  return fopen(name, "rb");
}

void AndroidPlatform::CloseFile(FILE* handle) {
  LOGI("CloseFile");
  fclose(handle);
}

char* AndroidPlatform::WritableFile(const char* name) {
  char* path = new char[100];
  path[0] = 0;

  strcat(path, "/sdcard/matongames/");
  //strcat(path, apkName);
  strcat(path, name);

  LOGI("WritableFile ");
  LOGI(path);

  return path;
}

