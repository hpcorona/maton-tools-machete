//
//  element.h
//  machete
//
//  Created by Hilario Perez Corona on 6/17/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "meta.h"

namespace machete {
  namespace draw {
    
    class TimedElement {
    public:
      TimedElement(float time, Element *element) { this->time = time; this->element = element; }
      inline float GetTime() { return time; }
      inline Element* GetElement() { return element; }
      
    protected:
      float time;
      Element *element;
    };

    class Container : public Element {
    public:
      Container();
      
      void Add(Element *child);
      void Remove(Element *child);
      void Switch(Element *prev, Element *elem);
      int Count() { return count; }
      
      void Invalidate();
      void Update(float time);
      void Draw(const Mat4 & matrix, Vec2 & pos, Vec4 & color, DrawContext *ctx);
      Iterator<Element*> *Iterator();
      
    protected:
      int count;
      machete::data::Iterator<Element*> childs;
    };
    
    class Root : public Container {
    public:
      Root(DrawContext *ctx);
      
      void Draw();
      
    protected:
      DrawContext *context;
      Mat4 matrix;
    };
    
    class Drawing : public Element {
    public:
      Drawing(MetaElement *e);
      void Invalidate();
      void Update(float time);
      void Draw(const Mat4 & matrix, Vec2 & pos, Vec4 & color, DrawContext *ctx);
      void SetFlipX(bool flipX) { this->flipX = flipX; }
      void SetFlipY(bool flipY) { this->flipY = flipY; }
      bool IsFlipX() const { return flipX; }
      bool IsFlipY() const { return flipY; }
      
    protected:
      bool flipX;
      bool flipY;
      MetaElement *element;
    };
    
    class Animation : public Element {
    public:
      Animation();
      void Add(float time, Element *child);

      void SetLoop(bool loop) { this->loop = loop; }
      inline bool IsLoop() const { return loop; }
      void Invalidate();
      void Update(float time);
      void Draw(const Mat4 & matrix, Vec2 & pos, Vec4 & color, DrawContext *ctx);
      void Restart();
      inline bool IsFinished() const { return finished; }
      
    protected:
      bool loop;
      bool finished;
      float time;
      float currTime;
      float totalTime;
      LinkedList<TimedElement*> *current;
      machete::data::Iterator<TimedElement*> frames;
    };
    
    class Actor : public Element {
    public:
      Actor();
      void Add(const Str & action, Animation *anim);
      inline Animation *GetFallback() const { return fallback; }
      inline Animation *GetCurrent() const { return current; }
      void ClearFallback() { fallback = NULL; }
      
      inline bool IsFinished() const;
      inline bool IsLoop() const;
      void SetFallback(const char* name);
      
      bool Play(const char* name);
      void Invalidate();
      void Update(float time);
      void Draw(const Mat4 & matrix, Vec2 & pos, Vec4 & color, DrawContext *ctx);
      void Restart();
      
    protected:
      Animation *fallback;
      Animation *current;
      Hash<Str, Animation*> actions;
    };
    
  }
}
