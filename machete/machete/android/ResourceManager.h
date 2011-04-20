#pragma once

#include <string.h>
#include "utils.h"
#include "vector.h"
#include "../engine.h"

class AndroidResourceManager : public IResourceManager {
public:
	AndroidResourceManager(const char* APKfile);
    ~AndroidResourceManager();
    void LoadImage(const char* filename);
    void* GetImageData();
    ivec2 GetImageSize();
    void UnloadImage();

private:
    zip* APKArchive;
    png_byte* imageData;
    ivec2 size;
};

