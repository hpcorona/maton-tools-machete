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

extern EAGLContext *context;
extern CAEAGLLayer *eaglLayer;

@interface MacheteGLView : UIView {
  machete::graphics::DrawContext* renEngine;
  float timestamp;
    
  machete::IGame *game;
}

- (void)drawView:(CADisplayLink*)displayLink;

@end
