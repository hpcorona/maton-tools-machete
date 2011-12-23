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



  LOGI("LoadAudio UNIMPLEMENTED");

  short audioFormat = ShortLE(data, 20);
  short channels = ShortLE(data, 22);
  int sampleRate = IntLE(data, 24);
  int byteRate = IntLE(data, 28);
  short bitsPerSample = ShortLE(data, 34);
  int dataSize = IntLE(data, 40);

  float duration = float(dataSize) / byteRate;

  unsigned char* buffData = (unsigned char*)(data + 44);

  ALuint buff;
  alGenBuffers(1, &buff);
  alBufferData(buff, channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, buffData, dataSize, sampleRate);

  //delete data;

  LOGI("Format: %d, %d, %d, %d, %d", audioFormat, channels, sampleRate, byteRate, bitsPerSample);
  /*
FILE* f = fopen("audio.wav", "fb");
char xbuffer[5];
xbuffer[5] = '\0';
if (fread(xbuffer, sizeof(char), 4, file) != 4 || strcmp(xbuffer, "RIFF") != 0)
	throw "Not a WAV file";

file_read_int32_le(xbuffer, file);

if (fread(xbuffer, sizeof(char), 4, file) != 4 || strcmp(xbuffer, "WAVE") != 0)
	throw "Not a WAV file";

if (fread(xbuffer, sizeof(char), 4, file) != 4 || strcmp(xbuffer, "fmt ") != 0)
	throw "Invalid WAV file";

file_read_int32_le(xbuffer, file);
short audioFormat = file_read_int16_le(xbuffer, file);
short channels = file_read_int16_le(xbuffer, file);
int sampleRate = file_read_int32_le(xbuffer, file);
int byteRate = file_read_int32_le(xbuffer, file);
file_read_int16_le(xbuffer, file);
short bitsPerSample = file_read_int16_le(xbuffer, file);

if (audioFormat != 16) {
	short extraParams = file_read_int16_le(xbuffer, file);
	file_ignore_bytes(file, extraParams);
}

if (fread(xbuffer, sizeof(char), 4, file) != 4 || strcmp(xbuffer, "data") != 0)
	throw "Invalid WAV file";

int dataChunkSize = file_read_int32_le(xbuffer, file);
unsigned char* bufferData = file_allocate_and_read_bytes(file, (size_t) dataChunkSize);

float duration = float(dataChunkSize) / byteRate;
alBufferData(buffer, GetFormatFromInfo(channels, bitsPerSample), bufferData, dataChunkSize, sampleRate);
free(bufferData);
fclose(f);
*/
  return buff;
}

inline unsigned int AndroidPlatform::Random() {
  return arc4random();
}

FILE* AndroidPlatform::OpenFile(const char* name) {
  LOGI("OpenFile FROM APK pending");
  LOGI(name);
  char fname[100];
  fname[0] = 0;
  strcat(fname, "/sdcard/matongames/assets/");
  strcat(fname, name);
  LOGI(fname);
  return fopen(fname, "rb");
}

void AndroidPlatform::CloseFile(FILE* handle) {
  LOGI("CloseFile FROM APK pending");
  fclose(handle);
}

char* AndroidPlatform::WritableFile(const char* name) {
  char* path = new char[100];
  path[0] = 0;

  strcat(path, "/sdcard/matongames/");
  //strcat(path, apkName);
  strcat(path, name);

  LOGI("WritableFile IN REAL FOLDER SDCARD PENDING");
  LOGI(path);

  return path;
}

