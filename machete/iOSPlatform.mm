
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "machete/engine.h"
#import <OpenAL/alc.h>
#import <OpenAL/al.h>
#import <AudioToolbox/AudioToolbox.h>

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
  
  bool LoadMusicInfo(const char* name, unsigned int &maxPSize, unsigned int &pCount, int & auFormat, int & freq) {
    NSString* basePath = [NSString stringWithUTF8String:name];
    
    NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
    
    NSURL* audioURL = [NSURL fileURLWithPath:fullPath];
    
    AudioFileID audioFile;
    OSStatus res = AudioFileOpenURL((CFURLRef)audioURL, kAudioFileReadPermission, 0, &audioFile);
    if (res != 0) {
      NSLog(@"Could not load: %@", fullPath);
      return false;
    }
    
    UInt32 maxPacketSize;
    UInt32 propSize = sizeof(maxPacketSize);
    UInt64 packetCount;
    UInt32 propSize64 = sizeof(packetCount);
    
    AudioFileGetProperty(audioFile, kAudioFilePropertyPacketSizeUpperBound, &propSize, &maxPacketSize);
    AudioFileGetProperty(audioFile, kAudioFilePropertyAudioDataPacketCount, &propSize64, &packetCount);
    
    maxPSize = maxPacketSize;
    pCount = packetCount;
    
    unsigned int total = maxPacketSize * packetCount;
    
    if (maxPSize == 4) {
      maxPSize = 1024;
      pCount = total / maxPSize;
      if (total % maxPSize > 0) {
        pCount += 1;
      }
    }
    
    AudioStreamBasicDescription format;
    propSize = sizeof(AudioStreamBasicDescription);
    
    AudioFileGetProperty(audioFile, kAudioFilePropertyDataFormat, &propSize, &format);
    
    auFormat = (format.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
    freq = (ALsizei)format.mSampleRate;
    
    AudioFileClose(audioFile);
    
    return true;
  }
  
  bool LoadMusicBuffers(const char* name, unsigned int maxPacketSize, unsigned int offset, unsigned int count, unsigned int &packsLoaded, void *audioData, unsigned int &bytesLoaded) {
    NSString* basePath = [NSString stringWithUTF8String:name];
    
    NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
    
    NSURL* audioURL = [NSURL fileURLWithPath:fullPath];
    
    AudioFileID audioFile;
    OSStatus res = AudioFileOpenURL((CFURLRef)audioURL, kAudioFileReadPermission, 0, &audioFile);
    if (res != 0) {
      NSLog(@"Could not load: %@", fullPath);
      return false;
    }
    
    SInt64 start = offset;
    UInt32 ioNumPackets = count;
    UInt32 ioBytes = count * maxPacketSize;
    
    AudioFileReadPacketData(audioFile, false, &ioBytes, NULL, start, &ioNumPackets, audioData);
    
    packsLoaded = ioNumPackets;
    bytesLoaded = ioBytes;
    
    AudioFileClose(audioFile);
    
    return true;
    
  }
  
  inline unsigned int Random() {
    return arc4random();
  }
  
  FILE* OpenFile(const char* name) {
    NSString* basePath = [NSString stringWithUTF8String:name];
    
    NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
    
    FILE* f = fopen([fullPath cStringUsingEncoding:NSASCIIStringEncoding], "rb");
    
    return f;
  }
  
  void CloseFile(FILE* handle) {
    fclose(handle);
  }
  
protected:
  CFDataRef imageData;
  
};
