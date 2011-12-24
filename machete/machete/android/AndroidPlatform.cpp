#include "AndroidPlatform.h"
#include <AL/al.h>
#include <AL/alc.h>

__BEGIN_DECLS
extern unsigned int arc4random(void);
__END_DECLS

machete::graphics::DrawContext* CreateDrawContext(machete::graphics::RenderTarget target, int width, int height) {

  machete::graphics::DrawContext *ctx = new machete::graphics::DrawContext(target);

  ctx->Initialize(width, height);

  return ctx;
}

AndroidPlatform::AndroidPlatform(const char* APKfile, const char* name) {
	LOGI("Initializing Android Platform with APK %s", APKfile);
	APKArchive = loadAPK(APKfile);
  apkFile = new char[100];
  apkFile[0] = 0;
  strcat(apkFile, APKfile);

  apkName = new char[100];
  apkName[0] = 0;
  strcat(apkName, APKfile);
  strcat(apkName, "/assets/");

  dirName = new char[100];
  dirName[0] = 0;
  strcat(dirName, name);

  imageData = NULL;
}

AndroidPlatform::~AndroidPlatform() {
	zip_close(APKArchive);
  delete apkName;
  delete dirName;
  delete apkFile;

  mems.Reset();
  while (mems.Next()) {
    char* m = mems.GetCurrent()->GetValue();
    delete m;
  }

  mems.RemoveAll();
/*
  resources.Reset();
  while (resources.Next()) {
    delete resources.GetCurrent()->GetValue();
  }

  resources.RemoveAll();
*/
}

void AndroidPlatform::SetResourceData(const char* name, long offset, long length) {
  struct AndroidResource *res = new AndroidResource();
  res->offset = offset;
  res->length = length;

  resources.Add(name, res);
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

	imageData = loadTextureFromPNG(APKArchive, pngfile, size.x, size.y);

  *data = (void*)imageData;
}

void AndroidPlatform::UnloadImage() {
  if (imageData == NULL) return;

	delete[] imageData;
	imageData = NULL;
}

unsigned int AndroidPlatform::LoadFile(const char* name, char** data) {
  int len = strlen(name);
  char* mname = new char[len+1];
  strcpy(mname, name);
  char* ext = mname + len - 4;

  if (len > 4) {
    if (strcmp(".xml", ext) == 0 || strcmp(".pos", ext) == 0 || strcmp(".fnt", ext) == 0) {
      strcpy(ext, ".mbd");
    }
  }

  char fname[100];
  fname[0] = 0;
  strcat(fname, "assets/");
  strcat(fname, mname);
  delete mname;

  unsigned int total = loadFile(APKArchive, fname, (void**)data);

  return total;
}

short ShortLE(char *data, int padding) {
  short v = *((short*)(data + padding));

//  v = (v>>8) | (v<<8);
  return v;
}

int IntLE(char *data, int padding) {
  int v = *((int*)(data + padding));
//  v = __builtin_bswap32(v);
  return v;
}

unsigned int AndroidPlatform::LoadAudio(const char* name) {
  char* data = NULL;
  unsigned int size = LoadFile(name, &data);

  if (size == 0) return 0;

  if (data[0] != 'R' || data[1] != 'I' || data[2] != 'F' || data[3] != 'F') {
    LOGI("Invalid File Format, not a RIFF WAV file %s", name);
    delete data;

    return 0;
  }

  short chunkSize = ShortLE(data, 16);

  if (chunkSize != 16) {
    LOGI("Should be 16 bytes PCM %s", name);
    delete data;

    return 0;
  }

  short audioFormat = ShortLE(data, 20);
  short channels = ShortLE(data, 22);
  int sampleRate = IntLE(data, 24);
  int byteRate = IntLE(data, 28);
  short bitsPerSample = ShortLE(data, 34);

  // By some weird bug of mine, IntLE(data, 40) is
  // not working, but i'm calculating the data size,
  // wich is: total data size - header size
  // where header size is 44
  int dataSize = size - 44;

  float duration = float(dataSize) / byteRate;

  unsigned char* buffData = (unsigned char*)(data + 44);

  ALuint buff;
  alGenBuffers(1, &buff);
  alBufferData(buff, channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, buffData, dataSize, sampleRate);

  // I cannot delete the buffer, because apparently
  // it is linked to OpenAL (it crashes if i delete it)
  // So, i'm appending it to a mems iterator so i
  // can unload it when the game is closing
  mems.Append(data);

  return buff;
}

inline unsigned int AndroidPlatform::Random() {
  return arc4random();
}

FILE* AndroidPlatform::OpenFile(const char* name, unsigned long &size) {
  machete::data::Tree<machete::data::Str, struct AndroidResource*> *node = resources.Seek(name);
  if (node == NULL) {
    LOGI("Not Found: %s", name);
    return NULL;
  }

  struct AndroidResource* res = node->GetValue();

  FILE *handle = fopen(apkFile, "rb");

  fseek(handle, res->offset, SEEK_CUR);

  size = res->length;

  return handle;
}

void AndroidPlatform::CloseFile(FILE* handle) {
  fclose(handle);
}

char* AndroidPlatform::WritableFile(const char* name) {
  char* path = new char[100];
  path[0] = 0;

  strcat(path, dirName);
  strcat(path, "/");
  strcat(path, name);

  return path;
}

