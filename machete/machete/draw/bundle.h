//
//  bundle.h
//  machete
//
//  Created by Hilario Perez Corona on 6/19/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

//! \file bundle.h
//! \brief A stiletto bundle loader and element creator.

#pragma once

#include "element.h"
#include "../data/mbd.h"
#include "font.h"
#include "../widget/widget.h"

using namespace machete::widget;
using namespace machete::data;
using namespace machete::graphics;

namespace machete {
  namespace draw {
    
    //! A metadata Bundle Image. Stores the necessary information to create a Drawing element.
    struct BdlImage {
      
      //! Create a new empty bundle image.
      BdlImage() { image = NULL; alpha = 1; rotation = 0; flipX = false; flipY = false; }
      
      //! The image associated.
      MetaSprite *image;
      
      //! The position or pivot.
      Vec2 position;
      
      //! The alpha/transparency.
      float alpha;
      
      //! The rotation in degrees.
      float rotation;
      
      //! If this needs to be flipped X.
      bool flipX;
      
      //! If this needs to be flipped Y.
      bool flipY;
    };
    
    //! A metadata Bundle Sprite. A sprite can consist of multiple images.
    struct BdlSprite {
      
      //! Create a new empty sprite.
      BdlSprite() { imageCount = 0; images = NULL; }
      
      //! Image count.
      int imageCount;
      
      //! Images for this sprite.
      machete::data::Iterator<struct BdlImage*> *images;
    };
    
    //! A metadata for a Timed Sprite. Maybe a frame?
    struct BdlTimed {
      
      //! Create a new timed sprite.
      BdlTimed() { time = 0; sprite = NULL; }
      
      //! The time.
      float time;
      
      //! The sprite.
      struct BdlSprite *sprite;
    };
    
    //! A metadata for an animation.
    struct BdlAnimation {
      
      //! Create a new bundle animation.
      BdlAnimation() { frames = NULL; }
      
      //! The frames.
      machete::data::Iterator<struct BdlTimed*> *frames;
    };
    
    //! The metadata for an action.
    struct BdlAction {
      
      //! Create a new action.
      BdlAction() { animation = NULL; }
      
      //! Name of the action.
      Str action;
      
      //! The animation linked to the action.
      struct BdlAnimation *animation;
    };
    
    //! The metadata for an actor.
    struct BdlActor {
      
      //! Create a new actor.
      BdlActor() { actions = NULL; }
      
      //! The actions that an actor can do.
      machete::data::Iterator<struct BdlAction*> *actions;
    };
    
    //! The metadata for a widget state.
    struct BdlWidgetState {
      
      //! Create a new state.
      BdlWidgetState() { framed = NULL; }
      
      //! Name of the state.
      Str state;
      
      //! The framed image linked to the state.
      MetaWidget *framed;
    };
    
    //! A metadata Widget. A widget can consist of multiple states.
    struct BdlWidget {
      
      //! Create a new widget.
      BdlWidget() { states = NULL; }
      
      //! The states that could be assigned to the widget.
      machete::data::Iterator<struct BdlWidgetState*> *states;
    };

    
    //! Stiletto Bundle Loader and Element creator.
    class Bundle {
    public:
      
      //! Creates a new bundle.
      /*!
       \param name File name of the stiletto bundle. The extension xml will be changed for mbd.
       */
      Bundle(const char* name);
      
      //! Destructor.
      ~Bundle() {}

      //! Get the image related to a name.
      /*!
       \param name Name of the image.
       \return The image requested. NULL if there is no image with that name.
       */
      MetaSprite* GetImage(const char* name) const;

      //! Get the framed image related to a name.
      /*!
       \param name Name of the framed image.
       \return The framed image requested. NULL if there is no framed image with that name.
       */
      MetaWidget* GetFramed(const char* name) const;
      
      //! Create a new Drawing based on an Image.
      /*!
       \param name Name of the image.
       \return A new Drawing with the image. NULL if there is no image with that name.
       */
      Drawing* NewImage(const char* name) const;
      
      //! Create a new Drawing based on an Image's metadata.
      /*!
       \param bimg Image metadata.
       \return A new Drawing with the image.
       */
      Drawing* NewDrawing(struct BdlImage* bimg) const;
      
      //! Create a new Sprite.
      /*!
       \param name Name of the bundle sprite.
       \return A new Element of type Drawing if the sprite only had one image, or a Container if it had more than one image. NULL if there is no sprite with that name in the bundle.
       */
      Element* NewSprite(const char* name) const;
      
      //! Create a new Sprite based on a Sprite's metadata.
      /*!
       \param spr Sprite metadata.
       \return A new Element of type Drawing if the sprite only had one image, or a Container if it had more than one image.
       */
      Element* NewSprite(struct BdlSprite* spr) const;
      
      //! Create a new Animation.
      /*!
       \param name Name of the bundle animation.
       \return A new Animation. NULL if there is no animation with that name in the bundle.
       */
      Animation* NewAnimation(const char* name) const;

      //! Create a new Animation based on an Animation's metadata.
      /*!
       \param banim Animation metadata.
       \return A new Animation.
       */
      Animation* NewAnimation(struct BdlAnimation* banim) const;
      
      //! Create a new Actor.
      /*!
       \param name Name of the bundle actor.
       \return A new Actor. NULL if there is no actor with that name in the bundle.
       */
      Actor* NewActor(const char* name) const;

      //! Create a new Actor based on an Actor's metadata.
      /*!
       \param bact Actor metadata.
       \return A new Actor.
       */
      Actor* NewActor(struct BdlActor *bact) const;
      
      //! Get the bundle font.
      /*!
       \param name Name of the font to request.
       \return The font, or NULL if not found.
       */
      Font* GetFont(const char* name) const;
      
      //! Create a new Widget.
      /*!
       \param name Name of the bundle widget.
       \return A new Widget. NULL if there is no widget with that name in the bundle.
       */
      Widget* NewWidget(const char* name) const;
      
      //! Create a new Widget based on an Widget's metadata.
      /*!
       \param bwdg Widget metadata.
       \return A new Widget.
       */
      Widget* NewWidget(struct BdlWidget *bwdg) const;
      
      //! Create a new Button.
      /*!
       \param name Name of the bundle button.
       \return A new Button. NULL if there is no widget with that name in the bundle.
       */
      Button* NewButton(const char* name) const;

      //! Create a new Button based on an Widget's metadata.
      /*!
       \param bwdg Widget metadata.
       \return A new Button.
       */
      Button* NewButton(struct BdlWidget *bwdg) const;
      
    protected:
      
      //! Load all texture atlases and build all the images.
      void LoadAtlas();
      
      //! Load all the sprites and build all the sprites metadata.
      void LoadSprites();
      
      //! Load all the animations and build all the animations metadata.
      void LoadAnimations();
      
      //! Load all actors and build all the actors metadata.
      void LoadActors();
      
      //! Load all the fonts and build them.
      void LoadFonts();
      
      //! Load all the framed images.
      void LoadFramed();
      
      //! Load all the widgets and build them.
      void LoadWidgets();
      
      //! The stiletto bundle dictionary.
      Mbd *bundle;
      
      //! The texture atlas dictionary.
      Mbd *atlas;
      
      //! The texture associated with this bundle.
      struct Tex *tex;
      
      //! Images by name.
      Hash<Str, MetaSprite*> images;
      
      //! Framed images by name.
      Hash<Str, MetaWidget*> framed;
      
      //! Sprite's metadata by name.
      Hash<Str, struct BdlSprite*> sprites;
      
      //! Animation's metadata by name.
      Hash<Str, struct BdlAnimation*> animations;
      
      //! Actor's metadata by name.
      Hash<Str, struct BdlActor*> actors;
      
      //! Widget's metadata by name.
      Hash<Str, struct BdlWidget*> widgets;
      
      //! Fonts by name.
      Hash<Str, Font*> fonts;
    };
    
  }
}
