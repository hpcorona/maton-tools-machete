//
//  MacheteGLView.m
//  machete
//
//  Created by Hilario Perez Corona on 4/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "MacheteGLView.h"
#import "iOSPlatform.mm"

EAGLContext *context = NULL;
CAEAGLLayer *eaglLayer = NULL;

@implementation MacheteGLView

- (id)initWithFrame:(CGRect)frame
{
  self = [super initWithFrame:frame];
  if (self) {
    machete::Start(new PlatformiOS());
    
    eaglLayer = (CAEAGLLayer*)super.layer;
    eaglLayer.opaque = YES;
    
    renEngine = CreateDrawContext(machete::graphics::TargetScreen, CGRectGetWidth(frame), CGRectGetHeight(frame));
    
    game = CreateGame();
    game->OnStart();
    game->OnResume();
    game->Initialize(renEngine, CGRectGetWidth(frame), CGRectGetHeight(frame), machete::DeviceOrientationPortrait);
    
    [self drawView:nil];
    timestamp = CACurrentMediaTime();
    
    CADisplayLink* displayLink;
    displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
    
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    
    [self setUserInteractionEnabled:YES];
    [self setMultipleTouchEnabled:NO]; // Solo un touch de momento
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

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
  [self sendTouches:touches withType:TouchStart];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  [self sendTouches:touches withType:TouchMove];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  [self sendTouches:touches withType:TouchEnd];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
  [self sendTouches:touches withType:TouchCancelled];
}

- (void)sendTouches:(NSSet *)touches withType:(TouchPhase) state {
  
  for (UITouch *touch in touches) {
    Touch *t = TheTouchInput->GetTouch(0);
    CGPoint pos = [touch locationInView:self];
    CGPoint prev = [touch previousLocationInView:self];
    
    if (state == TouchStart) {
      t->start.x = pos.x;
      t->start.y = pos.y;
    }
    t->phase = state;
    t->finger = 1;
    t->previous.x = prev.x;
    t->previous.y = prev.y;
    t->current.x = pos.x;
    t->current.y = pos.y;
    t->tapCount = touch.tapCount;
    t->offset = t->current - t->previous;
  }
  
  TheTouchInput->MarkAvailable();
}

@end

machete::graphics::DrawContext* CreateDrawContext(machete::graphics::RenderTarget target, int width, int height) {
  bool force = false;
  
  if (context == NULL) {
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
      return NULL;
    }
  }
  
  machete::graphics::DrawContext *renEngine = new machete::graphics::DrawContext(target);
  
  if (context.API == kEAGLRenderingAPIOpenGLES1) {
    NSLog(@"Using OpenGL ES 1.1");
    //renEngine = CreateRendereriOS1(target);
  } else {
    NSLog(@"Using OpenGL ES 2.0");
    //renEngine = CreateRendereriOS2(target);
  }

  if (target == machete::graphics::TargetScreen) {
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
  }

  renEngine->Initialize(width, height);

  return renEngine;
}
