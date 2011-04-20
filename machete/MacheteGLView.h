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

@interface MacheteGLView : UIView {
    ResourceManager *resMan;
    EAGLContext *context;
    IRenderingEngine* renEngine;
    float timestamp;
    
    IGame *game;
}

- (void)drawView:(CADisplayLink*)displayLink;

@end
