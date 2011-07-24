
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "machete/engine.h"

class PlatformiOS : public machete::IPlatform {
public:
  
  PlatformiOS() {
    imageData = NULL;
  }
  
  const char* GetResourcePath() const {
    NSString* bundlePath = [[NSBundle mainBundle] resourcePath];
    return [bundlePath UTF8String];
  }
  
  void LoadImage(const char* name, void **data, machete::math::IVec2 & size) {
    UnloadImage();
    
    NSString* basePath = [NSString stringWithUTF8String:name];
    NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
    UIImage* uiImage = [UIImage imageWithContentsOfFile:fullPath];
    CGImageRef cgImage = uiImage.CGImage;
    size.x = CGImageGetWidth(cgImage);
    size.y = CGImageGetHeight(cgImage);
    imageData = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
    
    *data = (void*)CFDataGetBytePtr(imageData);
  }
  
  void UnloadImage() {
    if (imageData != NULL) {
      CFRelease(imageData);
    }
    imageData = NULL;
  }
  
  unsigned int LoadFile(const char* name, char** data) {
    NSString* basePath = [NSString stringWithUTF8String:name];
    if ([basePath hasSuffix:@".xml"]) {
      basePath = [NSString stringWithFormat:@"%@.mbd", [basePath substringToIndex:[basePath length] - 4]];
    } else if ([basePath hasSuffix:@".lang"]) {
      basePath = [NSString stringWithFormat:@"%@.mbd", [basePath substringToIndex:[basePath length] - 5]];
    } else if ([basePath hasSuffix:@".fnt"]) {
      basePath = [NSString stringWithFormat:@"%@.mbd", [basePath substringToIndex:[basePath length] - 4]];
    } else if ([basePath hasSuffix:@".pos"]) {
      basePath = [NSString stringWithFormat:@"%@.mbd", [basePath substringToIndex:[basePath length] - 4]];
    }
    
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
  
  inline unsigned int Random() {
    unsigned int r = arc4random();
    Str logged("",10);
    logged += (int)r;
    machete::common::Log(logged);
    return r;
  }
  
protected:
  CFDataRef imageData;
  
};
