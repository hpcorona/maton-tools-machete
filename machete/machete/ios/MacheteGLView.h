//
//  MacheteGLView.h
//  machete
//
//  Created by Hilario Perez Corona on 4/18/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import <OpenGLES/EAGL.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import "machete/engine.h"

using namespace machete::input;

extern EAGLContext *context;
extern CAEAGLLayer *eaglLayer;

@interface MacheteGLView : UIView {
  machete::graphics::DrawContext* renEngine;
  double timestamp;
    
  machete::IGame *game;
}

- (void)drawView:(CADisplayLink*)displayLink;

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)sendTouches:(NSSet *)touches withType:(TouchPhase) state;

@end
