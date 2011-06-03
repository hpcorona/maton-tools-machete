
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
      
      NSError* err = nil;
      NSStringEncoding enc;
      
      NSString* content = [NSString stringWithContentsOfFile:fullPath usedEncoding:&enc error:&err];
      
      if (err != nil) {
        *data = 0;
        return 0;
      }
      
      unsigned int size = [content lengthOfBytesUsingEncoding:enc] + 1;
      *data = new char[size];
      data[size - 1] = 0;
      unsigned int total = size;
      
      [content getBytes:*data maxLength:size usedLength:&total encoding:enc options:nil range:NSMakeRange(0,size) remainingRange:nil];
      
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
