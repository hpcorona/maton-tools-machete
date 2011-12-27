#include "AndroidPlatform.h"

__BEGIN_DECLS
extern unsigned int arc4random(void);
__END_DECLS

machete::graphics::DrawContext* CreateDrawContext(machete::graphics::RenderTarget target, int width, int height) {

  machete::graphics::DrawContext *ctx = new machete::graphics::DrawContext(target);

  ctx->Initialize(width, height);

  return ctx;
}

AndroidPlatform::AndroidPlatform(const char* APKfile, const char* name) {
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

  OpenAPK();
}

AndroidPlatform::~AndroidPlatform() {
  delete apkName;
  delete dirName;
  delete apkFile;

  mems.Reset();
  while (mems.Next()) {
    char* m = mems.GetCurrent()->GetValue();
    delete m;
  }

  mems.RemoveAll();

  CloseAPK();
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

	imageData = LoadPNG(pngfile, size.x, size.y);

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

  zip_file* file = zip_fopen(apk, fname, 0);
  if (!file) {
    LOGE("Error opening %s from APK", fname);
    return 0;
  }

  unsigned int size = file->bytes_left;

  *data = new char[size];
  zip_fread(file, *data, size);

  zip_fclose(file);

  return size;
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
  /*
  char fname[100];
  fname[0] = 0;
  strcat(fname, "/mnt/sdcard/matongames/assets/");
  strcat(fname, name);

  FILE* handle = fopen(fname, "rb");
  */

  machete::data::Tree<machete::data::Str, struct AndroidResource*> *node = resources.Seek(name);
  if (node == NULL) {
    LOGI("Not Found: %s", name);
    return NULL;
  }

  struct AndroidResource* res = node->GetValue();

  FILE *handle = fopen(apkFile, "rb");

  fseek(handle, res->offset, SEEK_SET);

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

double AndroidPlatform::AbsoluteTime() {
  return time(NULL);
}

void AndroidPlatform::OpenAPK() {
  apk = zip_open(apkFile, 0, NULL);
  if (apk == NULL) {
    LOGE("Error loading APK");
  }
}

void AndroidPlatform::CloseAPK() {
  zip_close(apk);
}

zip_file* current_png_readed = NULL;

void png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length) {
  zip_fread(current_png_readed, data, length);
}

png_byte* AndroidPlatform::LoadPNG(const char *filename, int &width, int &height) {
  zip_file* file = zip_fopen(apk, filename, 0);
  if (!file) {
    LOGE("Error opening %s from APK", filename);
    return NULL;
  }

  //header for testing if it is a png
  png_byte header[8];

  //read the header
  zip_fread(file, header, 8);

  //test if png
  int is_png = !png_sig_cmp(header, 0, 8);
  if (!is_png) {
    zip_fclose(file);
    LOGE("Not a png file : %s", filename);
    return NULL;
  }

  //create png struct
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
      NULL, NULL);
  if (!png_ptr) {
    zip_fclose(file);
    LOGE("Unable to create png struct : %s", filename);
    return (NULL);
  }

  //create png info struct
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
    LOGE("Unable to create png info : %s", filename);
    zip_fclose(file);
    return (NULL);
  }

  //create png info struct
  png_infop end_info = png_create_info_struct(png_ptr);
  if (!end_info) {
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
    LOGE("Unable to create png end info : %s", filename);
    zip_fclose(file);
    return (NULL);
  }

  //png error stuff, not sure libpng man suggests this.
  if (setjmp(png_jmpbuf(png_ptr))) {
    zip_fclose(file);
    LOGE("Error during setjmp : %s", filename);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    return (NULL);
  }

  //init png reading
  //png_init_io(png_ptr, fp);
  current_png_readed = file;
  png_set_read_fn(png_ptr, NULL, png_zip_read);

  //let libpng know you already read the first 8 bytes
  png_set_sig_bytes(png_ptr, 8);

  // read all the info up to the image data
  png_read_info(png_ptr, info_ptr);

  //variables to pass to get info
  int bit_depth, color_type;
  png_uint_32 twidth, theight;

  // get info about png
  png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
      NULL, NULL, NULL);

  //update width and height based on png info
  width = twidth;
  height = theight;

  // Update the png info struct.
  png_read_update_info(png_ptr, info_ptr);

  // Row size in bytes.
  int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

  // Allocate the image_data as a big block, to be given to opengl
  png_byte *image_data = new png_byte[rowbytes * height];
  if (!image_data) {
    //clean up memory and close stuff
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    LOGE("Unable to allocate image_data while loading %s ", filename);
    zip_fclose(file);
    return NULL;
  }

  //row_pointers is for pointing to image_data for reading the png with libpng
  png_bytep *row_pointers = new png_bytep[height];
  if (!row_pointers) {
    //clean up memory and close stuff
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    delete[] image_data;
    LOGE("Unable to allocate row_pointer while loading %s ", filename);
    zip_fclose(file);
    return NULL;
  }
  // set the individual row_pointers to point at the correct offsets of image_data
  for (int i = 0; i < height; ++i)
    row_pointers[height - 1 - i] = image_data + i * rowbytes;

  //read the png into image_data through row_pointers
  png_read_image(png_ptr, row_pointers);

  //clean up memory and close stuff
  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
  delete[] row_pointers;
  zip_fclose(file);

  return image_data;
}
