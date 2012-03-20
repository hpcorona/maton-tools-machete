//
//  font.h
//  machete
//
//  Created by Hilario Perez Corona on 6/20/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

//! \file font.h
//! \brief Sprite font definitions.

#pragma once

#include "meta.h"
#include "../data/mbd.h"

using namespace machete::data;
using namespace machete::math;

namespace machete {
  namespace draw {
    
    //! A single character of a sprite font.
    class FontChar : public MetaSprite {
    public:
      
      //! Create a new character.
      //! Create a new sprite.
      /*!
       \param size The size of the sprite in pixels.
       \param uv0 The first coordinate of the texture.
       \param uv1 The second coordinate of the texture.
       \param texture The identifier of the texture.
       */
      FontChar(const Vec2 & pivot, const Vec2 & size, const Vec2 & uv0, const Vec2 & uv1, unsigned int texture);
      
      //! Changes the pixels that this character consumes in the x axis.
      /*!
       \param x The x pixels to advance.
       */
      void SetXAdvance(float x) { advance.x = x; }
      
      //! Changes the pixels that this character consumes in the y axis.
      /*!
       \param y The y pixels to advance.
       */
      void SetYAdvance(float y) { advance.y = y; }
      
      //! Get the pixels to advance after printing this character in the x axis.
      /*!
       \return The quantity of pixels to advance in the x axis.
       */
      inline float GetXAdvance() { return advance.x; }

      //! Get the pixels to advance after printing this character in the y axis.
      /*!
       \return The quantity of pixels to advance in the y axis.
       */
      inline float GetYAdvance() { return advance.y; }
      
      //! Changes the pixels that this character consumes in both axis.
      /*!
       \param adv The advancement in both axis.
       */
      void SetAdvance(const Vec2 & adv) { advance = adv; }
      
      //! Get the pixels to advance after printing this character in the x and y axis.
      /*!
       \return The quantity of pixels to advance in the x and y axis.
       */
      inline const Vec2 & GetAdvance() { return advance; }
      
      //! Get the pivot of the character.
      /*!
       \return The pivot of the character.
       */
      inline const Vec2 & GetPivot() { return pivot; }
      
    protected:
      //! Advancement of the x and y axis.
      Vec2 advance;
      
      //! Pivot of the character.
      Vec2 pivot;
    };
    
    //! A text element representing a series of characters printed sequentially to give the effect of text being drawn to screen.
    class Text : public Element {
    public:
      
      //! Create a new Text element with a maximum characters capacity.
      /*!
       \param cap The maximum capacity.
       */
      Text(int cap);
      
      //! Destructor.
      ~Text();
      
      //! Clear all current font characters. This element won't display anything.
      void Clear();
      
      //! Changes the character at one position.
      void SetChar(int idx, FontChar* fchar);
      
      //! Truncates the length of the text.
      void SetLength(int len);
      
      /*!
       When you change position of an object, they are not re-calculated. You need to call this function
       to recalculate the bounds.
       
       This is made this way for performance reasons.
       */
      void Invalidate();
      
      //! Updates this element.
      /*!
       \param time The time since the last update, in seconds.
       */
      void Update(float time);
      
      //! Draw the text to a drawing context.
      /*!
       \param matrix The current transformation matrix. If this will be changed, then you must restore it before returning from this method.
       \param pos Position or Offset to draw elements.
       \param color Tint to apply to all objects.
       \param ctx DrawContext to draw the elements.
       */
      void Draw(const Mat4 & matrix, Vec2 & pos, Vec4 & color, DrawContext *ctx);
      
      //! Get the maximum pivot of Y.
      float GetMaxPivot() const;
      
      //! Get the total text size.
      Vec2 & GetTextSize();
      
      //! Send a touch event to this element.
      /*!
       \param touch The touch event.
       \return True if the event was processed.
       */
      bool TouchEvent(machete::input::Touch *touch);

      
    protected:
      
      //! Characters to display.
      FontChar **chars;
      
      //! Maximum capacity in characters.
      int cap;
      
      //! Current number of characters to display.
      int len;
      
      //! Maximum Y pivot of the text.
      float maxPivot;
      
      //! Text size.
      Vec2 textSize;

    };
    
    //! A font, used to create text.
    class Font {
    public:
      
      //! Creates a font from a font definition file.
      /*!
       \param name File name to load. If the extension is xml or fnt it will be changed to mbd.
       \param t The texture.
       */
      Font(const char* name, struct Tex * t);
      
      //! Destructor.
      ~Font();
      
      //! Get the font character corresponding to a UTF-8 character.
      /*!
       \return The FontChar. Or NULL if not available for this font.
       */
      FontChar* Get(int c);
      
      //! Creates a new Text element with the specified string and maximum capacity.
      /*!
       \param str The string to display.
       \param cap The maximum capacity of the text.
       \return A new Text element.
       */
      Text* Create(Str & str, int cap);
      
      //! Changes the characters of a text element to match the characters of a string.
      /*!
       \param txt The text element.
       \param str The new text that we want to display.
       */
      void Change(Text *txt, Str & str);
      
    protected:
      //! Loads the font file.
      void LoadFont();
      
      //! The font dictionary.
      Mbd *font;
      
      //! The pool of characters available for this font.
      Hash<int, FontChar*> chars;
      
      //! The texture associated with this font.
      struct Tex *texture;
      
    };
    
  }
}
