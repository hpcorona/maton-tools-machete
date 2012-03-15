//
//  engine.h
//  machete
//
//  Created by Hilario Perez Corona on 4/10/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

//! \file engine.h
//! \brief Engine header. It will include everything you need.

#pragma once

#include "thread/thread.h"
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
#include "sound/sound.h"
#include "storage/sql.h"
#include "widget/extended.h"
#include "anim/func.h"

//! The main namespace. All the escential definitions of the engine are here.
namespace machete {
  
  using namespace math;
  
  //! Supported Device Orientations
  enum DeviceOrientation {
    DeviceOrientationUnknown,             //!< Device Orientation not available
    DeviceOrientationPortrait,            //!< Portrait Device Orientation
    DeviceOrientationPortraitUpsideDown,  //!< Upside Down Orientation
    DeviceOrientationLandscapeLeft,       //!< Device rotated Left
    DeviceOrientationLandscapeRight       //!< Device rotated Right
  };
  
  //! Game abstract class
  /*!
   You need to create an extension from this class to make your own game.
   
   The Machete engine will call the appropiate methods to run your game.
   */
  class IGame {
  public:
    
    virtual ~IGame() {}
    
    //! This is the first call to your game.
    /*!
		 Machete will send you the draw context so you can render images
		 to the screen.
		
     \param ctx The DrawContext asociated with the device's screen.
     \param width The width in pixels of the screen.
     \param height The height in pixels of the screen.
     \param devor The initial device orientation.
     */
    virtual void Initialize(graphics::DrawContext* ctx, int width, int height, DeviceOrientation devor) = 0;
    
    //! Called whenever the device is rotated or the screen resolution changes.
    /*!
     The screen width/height is ALWAYS in relation to DeviceOrientationPortrait.
     It is your duty to rotate your scene to match the new device orientation.
     
     \param width The new width of the screen in pixels.
     \param height The new height of the screen in pixels.
     \param devor The new device orientation.
     */
    virtual void Resize(int width, int height, DeviceOrientation devor) = 0;

		//! Called at the start of each frame.
		/*!
		 \param time The time in seconds since the last call to this function.
		 */
    virtual void Update(float time) = 0;

		//! Called after the Update funcion. This function must render the scene.
    virtual void Render() = 0;

		//! Called when the Operating System request a pause in your application.
		/*!
		 This can be caused by different reasons, for example, you are running
		 on an iPhone and the user receives a call. Or when running on Windows,
		 the game window has lost focus.
		
		 The pause can be long, so you should prepare and disconnect from sockets,
		 etc...
		 */
    virtual void OnPause() = 0;

		//! Called when your application will resume execution (restoring from a pause).
    virtual void OnResume() = 0;

		//! Called when your application is ready to start playing.
		/*!
		 Called just once in all your application's lifecycle.
		 */
    virtual void OnStart() = 0;

		//! Called when your application is about to close.
		/*!
		 On Movile devices (iOS and Android), usually your application
		 will live "forever", or until memory is required and your
		 application must give up it's memory.
		
		 This will be called only once on your application's lifecycle.
		 */
    virtual void OnStop() = 0;
  };

  //! Start the Engine. This is called automatically by each platform's glue code.
  static void Start(IPlatform *platf) {
    thread::TheThreadMgr = new thread::ThreadManager();
    
    platform::ThePlatform = platf;
    
    graphics::TheShaderMgr = new graphics::ShaderMgr();
    graphics::TheTextureMgr = new graphics::TextureMgr();
    sound::TheSoundMgr = new sound::SoundManager();
    sound::TheMusicMgr = new sound::MusicManager();
  }
  
}

//! Creator function. You need to implement this function.
/*!
You just need to return an instance of your game.

\return An instance of your game class.
 */
machete::IGame* CreateGame();
