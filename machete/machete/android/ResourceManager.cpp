#include "ResourceManager.h"

AndroidResourceManager::AndroidResourceManager(const char* APKfile) {
	LOGI("Initializing Resource Manager with APK %s", APKfile);
	APKArchive = loadAPK(APKfile);
}

AndroidResourceManager::~AndroidResourceManager() {
	zip_close(APKArchive);
}

void AndroidResourceManager::LoadImage(const char* filename) {
	char pngfile[100];
	pngfile[0] = 0;
	strcat(pngfile, "assets/");
	strcat(pngfile, filename);

	LOGI("open %s", filename);
	imageData = loadTextureFromPNG(APKArchive, pngfile, size.x, size.y);
}

void* AndroidResourceManager::GetImageData() {
	return imageData;
}

ivec2 AndroidResourceManager::GetImageSize() {
	return size;
}

void AndroidResourceManager::UnloadImage() {
	delete[] imageData;
	imageData = NULL;
}
