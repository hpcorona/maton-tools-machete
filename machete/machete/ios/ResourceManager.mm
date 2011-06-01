
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "../engine.h"

namespace machete {
    
class ResourceManager : public IResourceManager {
public:
    const char* GetResourcePath() const {
        NSString* bundlePath = [[NSBundle mainBundle] resourcePath];
        return [bundlePath UTF8String];
    }
    
    void LoadImage(const char* name) {
        NSString* basePath = [NSString stringWithUTF8String:name];
        NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
        NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
        UIImage* uiImage = [UIImage imageWithContentsOfFile:fullPath];
        CGImageRef cgImage = uiImage.CGImage;
        imageSize.x = CGImageGetWidth(cgImage);
        imageSize.y = CGImageGetHeight(cgImage);
        imageData = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
    }
    
    void* GetImageData() {
        return (void*)CFDataGetBytePtr(imageData);
    }
    
    ivec2 GetImageSize() {
        return imageSize;
    }
    
    void UnloadImage() {
        CFRelease(imageData);
    }
    
private:
    CFDataRef imageData;
    ivec2 imageSize;
};

}
