
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "machete/engine.h"

class PlatformiOS : public machete::IPlatform {
public:
  
  const char* GetResourcePath() const {
    NSString* bundlePath = [[NSBundle mainBundle] resourcePath];
    return [bundlePath UTF8String];
  }
  
  void LoadImage(const char* name, char **data, machete::math::IVec2 & size) {
    NSString* basePath = [NSString stringWithUTF8String:name];
    NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
    UIImage* uiImage = [UIImage imageWithContentsOfFile:fullPath];
    CGImageRef cgImage = uiImage.CGImage;
    size.x = CGImageGetWidth(cgImage);
    size.y = CGImageGetHeight(cgImage);
    CFDataRef imageData = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
    CFIndex dlen = CFDataGetLength(imageData);
    
    *data = new char[dlen];
    char *v = (char*)CFDataGetBytePtr(imageData);
    
    for (CFIndex i = 0; i < dlen; i++) {
      *data[i] = v[i];
    }
    
    CFRelease(imageData);
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
  
};
