//
//  IRenderingEngine.hpp
//  machete
//
//  Created by Hilario Perez Corona on 4/10/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#pragma once

#include "draw/bundle.h"
#include "platform/platform.h"
#include "graphics/shader.h"
#include "graphics/draw.h"
#include "math/vector.h"
#include "math/matrix.h"
#include "math/quaternion.h"
#include "data/str.h"
#include "data/tree.h"
#include "data/lists.h"
#include "data/mbd.h"
#include "common/log.h"
#include "input/touch.h"

namespace machete {
  
  using namespace math;
  enum DeviceOrientation {
    DeviceOrientationUnknown,
    DeviceOrientationPortrait,
    DeviceOrientationPortraitUpsideDown,
    DeviceOrientationLandscapeLeft,
    DeviceOrientationLandscapeRight
  };
  
  class IGame {
  public:
    virtual void Initialize(graphics::DrawContext*, int, int, DeviceOrientation) = 0;
    virtual void Resize(int, int, DeviceOrientation) = 0;
    virtual void Update(float time) = 0;
    virtual void Render() = 0;
    virtual void OnPause() = 0;
    virtual void OnResume() = 0;
    virtual void OnStart() = 0;
    virtual void OnStop() = 0;
  };
  
  static void Start(IPlatform *platf) {
    platform::ThePlatform = new platform::Platform(platf);
    
    graphics::TheShaderMgr = new graphics::ShaderMgr();
    graphics::TheTextureMgr = new graphics::TextureMgr();
  }
  
}

machete::IGame* CreateGame();
