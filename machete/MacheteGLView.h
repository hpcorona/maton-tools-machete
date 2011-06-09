//
//  MacheteGLView.h
//  machete
//
//  Created by Hilario Perez Corona on 4/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import <OpenGLES/EAGL.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import "machete/engine.h"
#import "machete/ios/ResourceManager.mm"

static EAGLContext *context = NULL;
static machete::ResourceManager *resMan = NULL;
static CAEAGLLayer *eaglLayer = NULL;

@interface MacheteGLView : UIView {
  machete::IRenderingEngine* renEngine;
  float timestamp;
    
  machete::IGame *game;
}

- (void)drawView:(CADisplayLink*)displayLink;

@end
