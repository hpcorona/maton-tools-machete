
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
    
    unsigned int LoadFile(const char* name, char** data) {
      NSString* basePath = [NSString stringWithUTF8String:name];
      NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
      NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
      
      NSData* content = [NSData dataWithContentsOfFile:fullPath];
      
      if (content == nil) {
        *data = 0;
        return 0;
      }
      
      unsigned int size = [content length];
      *data = new char[size];
      unsigned int total = size;
      
      [content getBytes:*data length:size];
      
      return total;
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
