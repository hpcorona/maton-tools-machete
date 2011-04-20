//
//  MacheteGLView.m
//  machete
//
//  Created by Hilario Perez Corona on 4/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "MacheteGLView.h"


@implementation MacheteGLView

- (id)initWithFrame:(CGRect)frame
{
    bool force = false;
    self = [super initWithFrame:frame];
    if (self) {
        resMan = new ResourceManager();
        
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*)super.layer;
        
        eaglLayer.opaque = YES;
        
        EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
        if (force) {
            api = kEAGLRenderingAPIOpenGLES1;
        }
        context = [[EAGLContext alloc] initWithAPI:api];
        if (!context && api == kEAGLRenderingAPIOpenGLES2) {
            api = kEAGLRenderingAPIOpenGLES1;
            context = [[EAGLContext alloc] initWithAPI:api];
        }
        
        if (!context || ![EAGLContext setCurrentContext:context]) {
            [self release];
            return nil;
        }
        
        if (api == kEAGLRenderingAPIOpenGLES1) {
            NSLog(@"Using OpenGL ES 1.1");
            renEngine = CreateRendereriOS1(resMan);
        } else {
            NSLog(@"Using OpenGL ES 2.0");
            renEngine = CreateRendereriOS2(resMan);
        }
        
        [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
        renEngine->Initialize(CGRectGetWidth(frame), CGRectGetHeight(frame));
        
        game = CreateGame(renEngine);
        game->OnStart();
        game->OnResume();
        game->Initialize(CGRectGetWidth(frame), CGRectGetHeight(frame), DeviceOrientationPortrait);
        
        [self drawView:nil];
        timestamp = CACurrentMediaTime();
        
        CADisplayLink* displayLink;
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
        
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
//        [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
//        
//        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didRotate:) name:UIDeviceOrientationDidChangeNotification object:nil];
    }
    return self;
}

- (void)dealloc
{
    game->OnStop();
    delete game;
    
    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }
    
    [context release];
    
    delete resMan;
    
    [super dealloc];
}

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (void)drawView: (CADisplayLink*) displayLink {
    float elapsedSeconds = 0;
    if (displayLink != nil) {
        elapsedSeconds = displayLink.timestamp - timestamp;
        timestamp = displayLink.timestamp;
    }
    
    game->Update(elapsedSeconds);
    game->Render();
    
    [context presentRenderbuffer:GL_RENDERBUFFER];
}

@end
