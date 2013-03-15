#include "emscripten.h"
#include <emscripten.h>


EmscriptenPlatform::EmscriptenPlatform() {
	imageData = NULL;
}
	
EmscriptenPlatform::~EmscriptenPlatform() {
	UnloadImage();
}

const char* EmscriptenPlatform::GetResourcePath() const {
	return "assets";
}

void EmscriptenPlatform::LoadImage(const char* filename, void **data, machete::math::IVec2 & size) {
  UnloadImage();
	
	char filename2[100];
	filename2[0] = 0;
	strcat(filename2, "assets/");
	strcat(filename2, filename);

	imageData = IMG_Load(filename2);
	if (!imageData) {
		size.x = 0;
		size.y = 0;
		*data = 0;
		return;
	}
	
	size.x = imageData->w;
	size.y = imageData->h;
	*data = (void*)imageData->pixels;
}

void EmscriptenPlatform::UnloadImage() {
  if (imageData == NULL) return;

	SDL_FreeSurface(imageData);
	imageData = NULL;
}

unsigned int EmscriptenPlatform::LoadFile(const char* name, char** data) {
  int len = strlen(name);
  char* mname = new char[len+1];
  strcpy(mname, name);
  char* ext = mname + len - 4;

  if (len > 4) {
    if (strcmp(".xml", ext) == 0 || strcmp(".pos", ext) == 0 || strcmp(".fnt", ext) == 0) {
      strcpy(ext, ".mbd");
    }
  }

	unsigned long size;
	FILE* file = OpenFile(mname, size);
	if (!file) {
		LOGE("Error opening %s from assets", mname);
		delete mname;
		return 0;
	}
	delete mname;

	*data = new char[size];
	fread(*data, 1, (unsigned int)size, file);
	CloseFile(file);
	
	return (unsigned int)size;
}

unsigned int EmscriptenPlatform::LoadAudio(const char* name) {
	return 0;
}

inline unsigned int EmscriptenPlatform::Random() {
	return (unsigned int)(emscripten_random() * 2147483647.0f);
}

FILE* EmscriptenPlatform::OpenFile(const char* name, unsigned long &size) {
  char fname[100];
  fname[0] = 0;
  strcat(fname, "assets/");
  strcat(fname, name);

  FILE* handle = fopen(fname, "rb");
	
	if (!handle) {
		size = 0;
	} else {
		fseek(handle, 0, SEEK_END);
		size = ftell(handle);
		fseek(handle, 0, SEEK_SET);
	}
	
	return handle;
}

void EmscriptenPlatform::CloseFile(FILE* handle) {
	fclose(handle);
}

char* EmscriptenPlatform::WritableFile(const char* name) {
	create_writable_file(name);
	
	char* newname = new char[strlen(name+3)];
	newname[0] = '/';
	newname[1] = 0;
	strcat(newname, name);
	
	return newname;
}

double EmscriptenPlatform::AbsoluteTime() {
	return time(NULL);
}

void EmscriptenPlatform::LaunchURL(const char * url) {
	// TODO: Nothing yet
}

void EmscriptenPlatform::Exit() {
	exit(0);
}
