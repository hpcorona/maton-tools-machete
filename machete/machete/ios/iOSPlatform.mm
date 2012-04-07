
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "machete/engine.h"
#import <OpenAL/alc.h>
#import <OpenAL/al.h>
#import <AudioToolbox/AudioToolbox.h>
#include <stdio.h>
#include <mach/mach_time.h>

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
    } else if ([basePath hasSuffix:@".wav"]) {
      basePath = [NSString stringWithFormat:@"%@.caf", [basePath substringToIndex:[basePath length] - 4]];
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
  
  unsigned int LoadAudio(const char* name) {
    NSString* basePath = [NSString stringWithUTF8String:name];
    if ([basePath hasSuffix:@".wav"]) {
      basePath = [NSString stringWithFormat:@"%@.caf", [basePath substringToIndex:[basePath length] - 4]];
    }
    
    NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
    
    NSURL* audioURL = [NSURL fileURLWithPath:fullPath];
    
    AudioFileID audioFile;
    OSStatus res = AudioFileOpenURL((CFURLRef)audioURL, kAudioFileReadPermission, 0, &audioFile);
    if (res != 0) {
      NSLog(@"Could not load: %@", fullPath);
      return 0;
    }
    
    AudioStreamBasicDescription format;
    UInt32 propSize = sizeof(AudioStreamBasicDescription);
    
    res = AudioFileGetProperty(audioFile, kAudioFilePropertyDataFormat, &propSize, &format);
    if (res != 0) {
      NSLog(@"Could not get properties: %@", fullPath);
      AudioFileClose(audioFile);
      return 0;
    }
    
    if (format.mChannelsPerFrame > 2) {
      NSLog(@"Unsupported more than 2 channels: %@", fullPath);
      AudioFileClose(audioFile);
      return 0;
    }
    
    if (format.mFormatID != kAudioFormatLinearPCM || !TestAudioFormatNativeEndian(format)) {
      NSLog(@"Unsupported, must be little-endien PCM: %@", fullPath);
      AudioFileClose(audioFile);
      return 0;
    }
    
    if (format.mBitsPerChannel != 8 && format.mBitsPerChannel != 16) {
      NSLog(@"Unsupported, must be 8 or 16 bit PCM: %@", fullPath);
      AudioFileClose(audioFile);
      return 0;
    }
    
    UInt64 fileSize;
    propSize = sizeof(UInt64);
    
    res = AudioFileGetProperty(audioFile, kAudioFilePropertyAudioDataByteCount, &propSize, &fileSize);
    if (res != 0) {
      NSLog(@"Could not get audio data size: %@", fullPath);
      AudioFileClose(audioFile);
      return 0;
    }
    
    UInt32 dataSize = (UInt32)fileSize;
    
    void* data = malloc(dataSize);
    if (data == nil) {
      NSLog(@"Could not allocate memory: %@", fullPath);
      AudioFileClose(audioFile);
      return 0;
    }
    
    res = AudioFileReadBytes(audioFile, false, 0, &dataSize, data);
    if (res != 0) {
      NSLog(@"Could not read the audio data: %@", fullPath);
      free(data);
      AudioFileClose(audioFile);
      return 0;
    }
    
    ALsizei size = (ALsizei)dataSize;
    ALenum auFormat = (format.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
    ALsizei freq = (ALsizei)format.mSampleRate;
    
    AudioFileClose(audioFile);
    
    // Create the OpenAL buffer
    ALuint buffer;
    alGenBuffers(1, &buffer);
    
    alBufferData(buffer, auFormat, data, size, freq);
    
    free(data);
    
    return buffer;
  }
  
  inline unsigned int Random() {
    return arc4random();
  }
  
  FILE* OpenFile(const char* name, unsigned long &size) {
    NSString* basePath = [NSString stringWithUTF8String:name];
    
    NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
    
    FILE* f = fopen([fullPath cStringUsingEncoding:NSASCIIStringEncoding], "rb");
    size = 0;
    
    return f;
  }
  
  void CloseFile(FILE* handle) {
    fclose(handle);
  }
  
  char* WritableFile(const char* name) {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    
    NSString *docsDir = [paths objectAtIndex:0];
    
    NSString *file = [NSString stringWithUTF8String:name];
    
    NSString *finalPath = [docsDir stringByAppendingPathComponent:file];
    
    const char *c = [finalPath cStringUsingEncoding:NSASCIIStringEncoding];
    
    char *b = new char[ [finalPath length] + 1 ];
    b[ [finalPath length] ] = 0;
    for (int i = 0; i < [finalPath length]; i++) {
      b[i] = c[i];
    }
    
    return b;
  }
  
  double AbsoluteTime() {
    return mach_absolute_time() / 1000000000L;
  }
  
  void LaunchURL(const char* url) {
    NSString *nsURL = [NSString stringWithUTF8String:url];
    
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:nsURL]];
  }
  
protected:
  CFDataRef imageData;
  
};
