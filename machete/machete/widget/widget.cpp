//
//  widget.cpp
//  machete
//
//  Created by Hilario Perez Corona on 7/3/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "widget.h"

namespace machete {
  namespace widget {
    
    MetaWidget::MetaWidget(const Vec2 & size, const Vec2 & uv0, const Vec2 & uv1, const Vec2 & topLeft, const Vec2 & bottomRight, unsigned int texture) {

      // Internal position of UV's
      Vec2 uvi0, uvi1, delta;
      delta = uv1 - uv0;
      
      this->size = size;
      this->drawSize = size;
      this->topLeft = topLeft;
      this->bottomRight = bottomRight;
      
      // Prepare the Internal UV positions
      uvi0.x = uv0.x + (topLeft.x / size.x) * delta.x;
      uvi0.y = uv0.y + (topLeft.y / size.y) * delta.y;
      
      uvi1.x = uv0.x + ((size.x - bottomRight.x) / size.x) * delta.x;
      uvi1.y = uv0.y + ((size.y - bottomRight.y) / size.y) * delta.y;
      
      // Top Left Box
      verts[0].vert.x = 0; verts[0].vert.y = 0; verts[0].vert.z = 0;
      verts[1].vert.x = 0; verts[1].vert.y = -topLeft.y; verts[1].vert.z = 0;
      verts[2].vert.x = topLeft.x; verts[2].vert.y = 0; verts[2].vert.z = 0;
      verts[3].vert.x = topLeft.x; verts[3].vert.y = -topLeft.y; verts[3].vert.z = 0;

      vertsV[0].vert.x = 0; vertsV[0].vert.y = 0; vertsV[0].vert.z = 0;
      vertsV[1].vert.x = 0; vertsV[1].vert.y = -topLeft.y; vertsV[1].vert.z = 0;
      vertsV[2].vert.x = topLeft.x; vertsV[2].vert.y = 0; vertsV[2].vert.z = 0;
      vertsV[3].vert.x = topLeft.x; vertsV[3].vert.y = -topLeft.y; vertsV[3].vert.z = 0;

      vertsH[0].vert.x = 0; vertsH[0].vert.y = 0; vertsH[0].vert.z = 0;
      vertsH[1].vert.x = 0; vertsH[1].vert.y = -topLeft.y; vertsH[1].vert.z = 0;
      vertsH[2].vert.x = topLeft.x; vertsH[2].vert.y = 0; vertsH[2].vert.z = 0;
      vertsH[3].vert.x = topLeft.x; vertsH[3].vert.y = -topLeft.y; vertsH[3].vert.z = 0;

      // Top Left UV
      verts[0].uv.x = uv0.x; verts[0].uv.y = uv0.y;
      verts[1].uv.x = uv0.x; verts[1].uv.y = uvi0.y;
      verts[2].uv.x = uvi0.x; verts[2].uv.y = uv0.y;
      verts[3].uv.x = uvi0.x; verts[3].uv.y = uvi0.y;

      vertsV[0].uv.x = uv0.x; vertsV[0].uv.y = uv0.y;
      vertsV[1].uv.x = uv0.x; vertsV[1].uv.y = uvi0.y;
      vertsV[2].uv.x = uvi0.x; vertsV[2].uv.y = uv0.y;
      vertsV[3].uv.x = uvi0.x; vertsV[3].uv.y = uvi0.y;

      vertsH[0].uv.x = uv0.x; vertsH[0].uv.y = uv0.y;
      vertsH[1].uv.x = uv0.x; vertsH[1].uv.y = uvi0.y;
      vertsH[2].uv.x = uvi0.x; vertsH[2].uv.y = uv0.y;
      vertsH[3].uv.x = uvi0.x; vertsH[3].uv.y = uvi0.y;

      // Top Right UV
      verts[4].uv.x = uvi1.x; verts[4].uv.y = uv0.y;
      verts[5].uv.x = uvi1.x; verts[5].uv.y = uvi0.y;
      verts[6].uv.x = uv1.x; verts[6].uv.y = uv0.y;
      verts[7].uv.x = uv1.x; verts[7].uv.y = uvi0.y;

      // Bottom Left UV
      verts[8].uv.x = uv0.x; verts[8].uv.y = uvi1.y;
      verts[9].uv.x = uv0.x; verts[9].uv.y = uv1.y;
      verts[10].uv.x = uvi0.x; verts[10].uv.y = uvi1.y;
      verts[11].uv.x = uvi0.x; verts[11].uv.y = uv1.y;

      vertsV[4].uv.x = uv0.x; vertsV[4].uv.y = uvi1.y;
      vertsV[5].uv.x = uv0.x; vertsV[5].uv.y = uv1.y;
      vertsV[6].uv.x = uvi0.x; vertsV[6].uv.y = uvi1.y;
      vertsV[7].uv.x = uvi0.x; vertsV[7].uv.y = uv1.y;

      // Bottom Right UV
      verts[12].uv.x = uvi1.x; verts[12].uv.y = uvi1.y;
      verts[13].uv.x = uvi1.x; verts[13].uv.y = uv1.y;
      verts[14].uv.x = uv1.x; verts[14].uv.y = uvi1.y;
      verts[15].uv.x = uv1.x; verts[15].uv.y = uv1.y;

      vertsH[4].uv.x = uvi1.x; vertsH[4].uv.y = uvi1.y;
      vertsH[5].uv.x = uvi1.x; vertsH[5].uv.y = uv1.y;
      vertsH[6].uv.x = uv1.x; vertsH[6].uv.y = uvi1.y;
      vertsH[7].uv.x = uv1.x; vertsH[7].uv.y = uv1.y;

      this->texture = texture;
      
      // Vertexes are aranged in this way:
      // 
      // 0  2    4  6
      // 1  3    5  7
      //
      // 8  10   12 14
      // 9  11   13 15
      
      // The elements are ordered
      // in this way:
      //
      // 1 2 3
      // 4 5 6
      // 7 8 9
      
      elems[0] = 0;
      elems[1] = 1;
      elems[2] = 2;
      elems[3] = 2;
      elems[4] = 1;
      elems[5] = 3;
      
      elems[6] = 2;
      elems[7] = 3;
      elems[8] = 4;
      elems[9] = 4;
      elems[10] = 3;
      elems[11] = 5;

      elems[12] = 4;
      elems[13] = 5;
      elems[14] = 6;
      elems[15] = 6;
      elems[16] = 5;
      elems[17] = 7;

      elems[18] = 1;
      elems[19] = 8;
      elems[20] = 3;
      elems[21] = 3;
      elems[22] = 8;
      elems[23] = 10;

      elems[24] = 3;
      elems[25] = 10;
      elems[26] = 5;
      elems[27] = 5;
      elems[28] = 10;
      elems[29] = 12;

      elems[30] = 5;
      elems[31] = 12;
      elems[32] = 7;
      elems[33] = 7;
      elems[34] = 12;
      elems[35] = 14;
      
      elems[36] = 8;
      elems[37] = 9;
      elems[38] = 10;
      elems[39] = 10;
      elems[40] = 9;
      elems[41] = 11;
      
      elems[42] = 10;
      elems[43] = 11;
      elems[44] = 12;
      elems[45] = 12;
      elems[46] = 11;
      elems[47] = 13;
      
      elems[48] = 12;
      elems[49] = 13;
      elems[50] = 14;
      elems[51] = 14;
      elems[52] = 13;
      elems[53] = 15;
      
      // Vertical Display, arranged in this way:
      // 0 1
      // 2 3
      // 
      // 4 5
      // 6 7
      
      elemsV[0] = 0;
      elemsV[1] = 1;
      elemsV[2] = 2;
      elemsV[3] = 2;
      elemsV[4] = 1;
      elemsV[5] = 3;
      
      elemsV[6] = 1;
      elemsV[7] = 4;
      elemsV[8] = 3;
      elemsV[9] = 3;
      elemsV[10] = 4;
      elemsV[11] = 6;
      
      elemsV[12] = 4;
      elemsV[13] = 5;
      elemsV[14] = 6;
      elemsV[15] = 6;
      elemsV[16] = 5;
      elemsV[17] = 7;
      
      // Horizontal Display, aranged in this way:
      // 
      // 0  2    4  6
      // 1  3    5  7
      //

      elemsH[0] = 0;
      elemsH[1] = 1;
      elemsH[2] = 2;
      elemsH[3] = 2;
      elemsH[4] = 1;
      elemsH[5] = 3;

      elemsH[6] = 2;
      elemsH[7] = 3;
      elemsH[8] = 4;
      elemsH[9] = 4;
      elemsH[10] = 3;
      elemsH[11] = 5;
      
      elemsH[12] = 4;
      elemsH[13] = 5;
      elemsH[14] = 6;
      elemsH[15] = 6;
      elemsH[16] = 5;
      elemsH[17] = 7;
    }
    
    void MetaWidget::Draw(DrawContext *ctx, const Vec2 & pivot, const Vec2 & pos, const Vec2 & scale, const Vec4 & color, float rotation, bool flipX, bool flipY) {
      
      if (!flipX && !flipY) {
        // Top Right Box
        verts[4].vert.x = drawSize.x - bottomRight.x; verts[4].vert.y = 0; verts[4].vert.z = 0;
        verts[5].vert.x = drawSize.x - bottomRight.x; verts[5].vert.y = -topLeft.y; verts[5].vert.z = 0;
        verts[6].vert.x = drawSize.x; verts[6].vert.y = 0; verts[6].vert.z = 0;
        verts[7].vert.x = drawSize.x; verts[7].vert.y = -topLeft.y; verts[7].vert.z = 0;

        // Bottom Left Box
        verts[8].vert.x = 0; verts[8].vert.y = -(drawSize.y - bottomRight.y); verts[8].vert.z = 0;
        verts[9].vert.x = 0; verts[9].vert.y = -drawSize.y; verts[9].vert.z = 0;
        verts[10].vert.x = topLeft.x; verts[10].vert.y = -(drawSize.y - bottomRight.y); verts[10].vert.z = 0;
        verts[11].vert.x = topLeft.x; verts[11].vert.y = -drawSize.y; verts[11].vert.z = 0;

        // Bottom Right Box
        verts[12].vert.x = drawSize.x - bottomRight.x; verts[12].vert.y = -(drawSize.y - bottomRight.y); verts[12].vert.z = 0;
        verts[13].vert.x = drawSize.x - bottomRight.x; verts[13].vert.y = -drawSize.y; verts[13].vert.z = 0;
        verts[14].vert.x = drawSize.x; verts[14].vert.y = -(drawSize.y - bottomRight.y); verts[14].vert.z = 0;
        verts[15].vert.x = drawSize.x; verts[15].vert.y = -drawSize.y; verts[15].vert.z = 0;
        
        for (int i = 0; i < 16; i++) {
          verts[i].pivot.x = pivot.x; verts[i].pivot.y = pivot.y;
          
          verts[i].offset.x = pos.x; verts[i].offset.y = pos.y;
          
          verts[i].color.x = color.x; verts[i].color.y = color.y; verts[i].color.z = color.z; verts[i].color.w = color.w;
          
          verts[i].scale.x = scale.x; verts[i].scale.y = scale.y;
          
          verts[i].rotation = rotation;
        }
        
        ctx->Draw(verts, 16, elems, 54, texture);
      } else if (flipX && !flipY) {
        // Top Box
        vertsH[4].vert.x = drawSize.x - bottomRight.x; vertsH[4].vert.y = 0; vertsH[4].vert.z = 0;
        vertsH[5].vert.x = drawSize.x - bottomRight.x; vertsH[5].vert.y = -topLeft.y; vertsH[5].vert.z = 0;
        vertsH[6].vert.x = drawSize.x; vertsH[6].vert.y = 0; vertsH[6].vert.z = 0;
        vertsH[7].vert.x = drawSize.x; vertsH[7].vert.y = -topLeft.y; vertsH[7].vert.z = 0;
        
        for (int i = 0; i < 8; i++) {
          vertsH[i].pivot.x = pivot.x; vertsH[i].pivot.y = pivot.y;
          
          vertsH[i].offset.x = pos.x; vertsH[i].offset.y = pos.y;
          
          vertsH[i].color.x = color.x; vertsH[i].color.y = color.y; vertsH[i].color.z = color.z; vertsH[i].color.w = color.w;
          
          vertsH[i].scale.x = scale.x; vertsH[i].scale.y = scale.y;
          
          vertsH[i].rotation = rotation;
        }
        
        ctx->Draw(vertsH, 8, elemsH, 18, texture);
      } else if (flipY && !flipX) {
        // Bottom Box
        vertsV[4].vert.x = 0; vertsV[4].vert.y = -(drawSize.y - bottomRight.y); vertsV[4].vert.z = 0;
        vertsV[5].vert.x = 0; vertsV[5].vert.y = -drawSize.y; vertsV[5].vert.z = 0;
        vertsV[6].vert.x = topLeft.x; vertsV[6].vert.y = -(drawSize.y - bottomRight.y); vertsV[6].vert.z = 0;
        vertsV[7].vert.x = topLeft.x; vertsV[7].vert.y = -drawSize.y; vertsV[7].vert.z = 0;

        for (int i = 0; i < 8; i++) {
          vertsV[i].pivot.x = pivot.x; vertsV[i].pivot.y = pivot.y;
          
          vertsV[i].offset.x = pos.x; vertsV[i].offset.y = pos.y;
          
          vertsV[i].color.x = color.x; vertsV[i].color.y = color.y; vertsV[i].color.z = color.z; vertsV[i].color.w = color.w;
          
          vertsV[i].scale.x = scale.x; vertsV[i].scale.y = scale.y;
          
          vertsV[i].rotation = rotation;
        }
        
        ctx->Draw(vertsV, 8, elemsV, 18, texture);
      }
    }
    
    Vec2 & MetaWidget::GetSize() {
      return size;
    }
    
    Vec2 & MetaWidget::GetDrawSize() {
      return drawSize;
    }
    
    void MetaWidget::SetDrawSize(const Vec2 &drawSize) {
      this->drawSize.x = cmax(drawSize.x, topLeft.x + bottomRight.x);
      this->drawSize.y = cmax(drawSize.y, topLeft.y + bottomRight.y);
    }
    
    void MetaWidget::Reset() {
      drawSize = size;
    }
    
    Widget::Widget() : touchProc(this) {
      size.x = 100;
      size.y = 100;
      state = NULL;
      event = NULL;
      display = WidgetNormal;
      
      allowDragY = false;
      allowDragX = false;
      allowTap = false;
    }
    
    Widget::Widget(const Vec2 & size) : touchProc(this) {
      this->size = size;
      state = NULL;
      event = NULL;
      display = WidgetNormal;

      allowDragY = false;
      allowDragX = false;
      allowTap = false;
    }
    
    void Widget::Add(const Str & state, MetaWidget *meta) {
      states.Add(state, meta);
    }
    
    MetaWidget *Widget::GetCurrent() const {
      return state;
    }
    
    void Widget::Clear() {
      state = NULL;
    }
    
    void Widget::SetState(const char* name) {
      Tree<Str, MetaWidget*> *node = states.Seek(name);
      if (node == NULL) {
        state = NULL;
        return;
      }
      
      state = node->GetValue();
    }
    
    void Widget::Invalidate() {
      Container::Invalidate();
      
      bounds.size += size;
    }
    
    void Widget::Update(float time) {
      touchProc.Update(time);
      
      Container::Update(time);
    }
    
    void Widget::Draw(const Mat4 & matrix, Vec2 & pos, Vec4 & color, DrawContext *ctx) {
      if (count == 0 && state == NULL) return;
      
      Mat4 mat = matrix;
      bool changed = false;
      
      if (scale.x != 1 || scale.y != 1 || rotation != 0) {
        
        ctx->Draw();
        
        ctx->ChangeModelView(Mat4().Translate(position.x + pos.x, position.y + pos.y, 0).Scale(scale.x, scale.y, 0).Rotate(rotation).Pointer());
        changed = true;
      }
      
      childs.Reset();
      
      Vec4 NewColor = this->color * color;
      Vec2 Position = position + pos;
      
      if (state != NULL) {
        state->SetDrawSize(size);
        
        bool dVert = display == WidgetVertical;
        bool dHorz = display == WidgetHorizontal;
        
        if (changed) {
          state->Draw(ctx, ZERO2, ZERO2, ONE2, NewColor, 0, dHorz, dVert);
        } else {
          state->Draw(ctx, ZERO2, Position, scale, NewColor, rotation, dHorz, dVert);
        }
      }
      
      while (childs.Next()) {
        Element *current = childs.GetCurrent()->GetValue();
        
        if (current->IsVisible()) {
          if (changed) {
            current->Draw(mat, ZERO2, NewColor, ctx);
          } else {
            current->Draw(mat, Position, NewColor, ctx);
          }
        }
      }
      
      if (changed) {
        ctx->Draw();
        ctx->ChangeModelView(matrix);
      }
    }
    
    void Widget::SetSize(float width, float height) {
      size.x = width;
      size.y = height;
      
      Invalidate();
    }
    
    void Widget::SetSize(const Vec2 & size) {
      this->size = size;
      
      Invalidate();
    }
    
    Vec2 & Widget::GetSize() {
      return size;
    }
    
    bool Widget::TouchEvent(machete::input::Touch *touch) {
      if (Container::TouchEvent(touch) == true) {
        return true;
      }
      
      Rect2D bounds = GetGlobalBounds();
      
      return touchProc.Gather(touch, bounds);
    }
    
    bool Widget::TouchAcceptTap() {
      return allowTap;
    }
    
    bool Widget::TouchTapIntent() {
      return allowTap;
    }
    
    void Widget::TouchTapCancelled() {
      
    }
    
    void Widget::TouchTapPerformed() {
      if (event != NULL) {
        event->WidgetTapped(this);
      }
    }
    
    bool Widget::TouchAcceptDragX() {
      return allowDragX;
    }

    bool Widget::TouchAcceptDragY() {
      return allowDragY;
    }

    void Widget::TouchDrag(Vec2 & move) {
      if (event != NULL) {
        event->WidgetDragged(this, move);
      }
    }
    
    void Widget::TouchInertia(Vec2 & move) {
      if (event != NULL) {
        event->WidgetInertia(this, move);
      }
    }
    
    void Widget::TouchEnded() {
      
    }
    
    void Widget::SetEventListener(WidgetEventAdapter *event) {
      this->event = event;
    }
    
    void Widget::SetAllowTap(bool allow) {
      allowTap = allow;
    }
    
    void Widget::SetAllowDragX(bool allow) {
      allowDragX = allow;
    }
    
    void Widget::SetAllowDragY(bool allow) {
      allowDragY = allow;
    }
    
    WidgetEventAdapter *Widget::GetEventListener() {
      return event;
    }
    
    void Widget::SetDisplay(WidgetDisplay type) {
      display = type;
    }
    
    Button::Button() {
      font = NULL;
      
      label = new Text(30);
      
      Container::Add(label);
      
      allowTap = true;
    }
    
    Button::~Button() {
      delete label;
    }
    
    void Button::SetLabel(Str & label) {
      if (font == NULL) {
        this->label->Clear();
        return;
      }
      
      font->Change(this->label, label);
      
      Invalidate();
    }
    
    void Button::SetLabel(const char* label) {
      if (font == NULL) {
        this->label->Clear();
        return;
      }
      
      Str lblText(label);
      SetLabel(lblText);
    }
    
    void Button::SetFont(Font *font) {
      this->font = font;
    }
    
    bool Button::TouchTapIntent() {
      SetState("pressed");
      touchProc.Acquiere(this);
      
      Widget::TouchTapIntent();
      
      return true;
    }
    
    void Button::TouchTapCancelled() {
      SetState("normal");
      touchProc.Release();

      Widget::TouchTapCancelled();
    }
    
    void Button::TouchTapPerformed() {
      SetState("normal");
      touchProc.Release();
      
      Widget::TouchTapPerformed();
    }
    
    void Button::TouchEnded() {
      SetState("normal");
      touchProc.Release();
    }
    
    void Button::Invalidate() {
      label->Invalidate();
      
      Widget::Invalidate();
      
      Vec2 tSize = label->GetTextSize();
      Vec2 size = GetSize();
      
      float x = (size.x - tSize.x) / 2.0f;
      float y = (size.y - tSize.y) / 2.0f - label->GetMaxPivot();
      
      label->SetPosition(x, y);
    }

    Scroll::Scroll(int width, int height) {
      frame = NULL;
      vScroll = NULL;
      hScroll = NULL;
      
      size.x = width;
      size.y = height;
      viewport = new Dynamic(width, height, size);
      viewport->SetUsingTexture(true);
      
      container = new Container();
      viewport->Add(container);
      viewport->SetParent(this);

      SetSize(width, height);

      margin.x = 5;
      margin.y = 5;
      
      elastic.x = 0;
      elastic.y = 0;
      
      drawVScroll = true;
      drawHScroll = true;
      
      allowVScroll = true;
      allowHScroll = true;
      
      autoVScroll = false;
      autoHScroll = false;
      
      freeDrag = true;
      
      centered = false;
      centering = false;
      
      allowDragY = true;
      allowDragX = true;
    }
    
    Scroll::~Scroll() {
      delete frame;
      delete vScroll;
      delete hScroll;
      delete container;
    }
    
    void Scroll::Invalidate() {
      bounds.Clear();
      bounds.pos = position;
      bounds.size = size;
      
      container->Invalidate();
    }
    
    void Scroll::Draw(const Mat4 &matrix, Vec2 &pos, Vec4 &color, machete::graphics::DrawContext *ctx) {
      if (!visible) return;
      
      Mat4 mat = matrix;
      bool changed = false;
      
      if (scale.x != 1 || scale.y != 1 || rotation != 0) {
        ctx->Draw();
        
        ctx->ChangeModelView(Mat4().Translate(position.x + pos.x, position.y + pos.y, 0).Scale(scale.x, scale.y, 0).Rotate(rotation).Pointer());
        changed = true;
      }
      
      childs.Reset();
      
      Vec4 NewColor = this->color * color;
      Vec2 Position = position + pos;
      
      while (childs.Next()) {
        if (changed) {
          childs.GetCurrent()->GetValue()->Draw(mat, ZERO2, NewColor, ctx);
        } else {
          childs.GetCurrent()->GetValue()->Draw(mat, Position, NewColor, ctx);
        }
      }
      
      if (changed) {
        viewport->Draw(mat, ZERO2, NewColor, ctx);
        
        InternalDraw(mat, ZERO2, NewColor, ctx);
      } else {
        viewport->Draw(mat, Position, NewColor, ctx);
        
        InternalDraw(mat, Position, NewColor, ctx);
      }
      
      if (changed) {
        ctx->Draw();
        ctx->ChangeModelView(matrix);
      }
    }
    
    void Scroll::InternalDraw(const Mat4 &matrix, Vec2 &pos, Vec4 &color, machete::graphics::DrawContext *ctx) {
      
      if (frame != NULL) {
        frame->Draw(matrix, pos, color, ctx);
      }
      
      if (!drawHScroll && !drawVScroll) return;
      
      Rect2D cbounds = container->GetBounds();
      
      float ex = abs(elastic.x);
      float ey = abs(elastic.y);
      float factorV = cmin(size.y / (cbounds.size.y + ey), 1);
      float factorH = cmin(size.x / (cbounds.size.x + ex), 1);

      float posV = -container->position.y / cbounds.size.y;
      float posH = -container->position.x / cbounds.size.x;
      
      if (posV < 0) {
        posV = 0;
      }
      
      if (posH < 0) {
        posH = 0;
      }
      
      float maxV = bounds.size.y - margin.y;
      float maxH = bounds.size.x - margin.x;
      
      float posY = maxV * posV;
      float height = maxV * factorV;
      
      float posX = maxH * posH;
      float width = maxH * factorH;

      if (vScroll != NULL && drawVScroll) {
        if (posY + height > bounds.size.y - margin.y) {
          posY = bounds.size.y - margin.y - height;
        }
        
        vScroll->SetPosition(bounds.size.x - margin.x, posY);
        vScroll->SetSize(margin.x, height);
        
        vScroll->Draw(matrix, pos, color, ctx);
      }
      
      if (hScroll != NULL && drawHScroll) {
        if (posX + width > bounds.size.x - margin.x) {
          posX = bounds.size.x - margin.x - width;
        }

        hScroll->SetPosition(posX, bounds.size.y - margin.y);
        hScroll->SetSize(width, margin.y);
        
        hScroll->Draw(matrix, pos, color, ctx);
      }
    }
    
    void Scroll::Add(Element *child) {
      container->Add(child);
    }
    
    void Scroll::Remove(Element *child) {
      container->Remove(child);
    }
    
    void Scroll::Switch(Element *prev, Element *elem) {
      container->Switch(prev, elem);
    }
    
    int Scroll::Count() const {
      return container->Count();
    }
    
    void Scroll::Update(float time) {
      if (!touchProc.IsTracking() && centering && !centered) {
        center.x = -container->position.x + size.x * 0.5f;
        center.y = -container->position.y + size.y * 0.5f;
        
        StepTarget(time);
      }

      touchProc.Update(time);
      viewport->Update(time);
      
      viewport->Draw();
      
      if (touchProc.IsTracking() == false && touchProc.IsAlive() && (elastic.x != 0 || elastic.y != 0)) {
        Vec2 strength(-elastic.x * SCROLL_ELASTICITY * time, -elastic.y * SCROLL_ELASTICITY * time);
        
        if (elastic.x > 0) {
          if (elastic.x + strength.x < 0) {
            strength.x = -elastic.x;
          }
        } else if (elastic.x < 0) {
          if (elastic.x + strength.x > 0) {
            strength.x = -elastic.x;
          }
        }

        if (elastic.y > 0) {
          if (elastic.y + strength.y < 0) {
            strength.y = -elastic.y;
          }
        } else if (elastic.y < 0) {
          if (elastic.y + strength.y > 0) {
            strength.y = -elastic.y;
          }
        }

        ElasticMovement(strength);
      }
      
      CalculateElastic(time);
      
      if (centering == false && touchProc.IsAlive() == false && touchProc.IsTracking() == false) {
        if (autoVScroll && vScroll != NULL) {
          vScroll->color.w -= time;
          if (vScroll->color.w < 0) {
            vScroll->color.w = 0;
          }
        }

        if (autoHScroll && hScroll != NULL) {
          hScroll->color.w -= time;
          if (hScroll->color.w < 0) {
            hScroll->color.w = 0;
          }
        }
      }
      
      center.x = -container->position.x + size.x * 0.5f;
      center.y = -container->position.y + size.y * 0.5f;

      if (centering) {
        if (cabs(center.x - targetGlue.x) < 1.0f && cabs(center.y - targetGlue.y) < 1.0f) {
          centering = false;
          centered = true;
        }
      }
      
      if (!freeDrag && !centering && !centered && !touchProc.IsTracking()) {
        SeekGluePoint(lastInertia);
      }
      
      lastInertia.x = 0; lastInertia.y = 0;
    }
    
    void Scroll::SetDecorators(machete::widget::Widget *frame, machete::widget::Widget *vScroll, machete::widget::Widget *hScroll) {
      
      this->frame = frame;
      this->vScroll = vScroll;
      this->hScroll = hScroll;
    }

    void Scroll::TouchDrag(Vec2 & move) {
      centered = false;
      centering = false;
      
      if (allowHScroll && allowVScroll) {
        Vec2 np = container->position + move;
      
        container->SetPosition(np);
      } else if (allowHScroll) {
        Vec2 np = container->position;
        np.x += move.x;
        
        container->SetPosition(np);
      } else if (allowVScroll) {
        Vec2 np = container->position;
        np.y += move.y;
        
        container->SetPosition(np);
      }
      
      if (autoVScroll && vScroll != NULL) {
        vScroll->color.w = 1;
      }

      if (autoHScroll && hScroll != NULL) {
        hScroll->color.w = 1;
      }
    }
    
    void Scroll::TouchInertia(Vec2 & move) {
      lastInertia = move;

      ElasticMovement(move);
    }
    
    void Scroll::ElasticMovement(Vec2 & move) {
      centered = false;
      
      if (allowHScroll && allowVScroll) {
        Vec2 np = container->position + move;
        
        container->SetPosition(np);
      } else if (allowHScroll) {
        Vec2 np = container->position;
        np.x += move.x;
        
        container->SetPosition(np);
      } else if (allowVScroll) {
        Vec2 np = container->position;
        np.y += move.y;
        
        container->SetPosition(np);
      }
      
      if (autoVScroll && vScroll != NULL) {
        vScroll->color.w = 1;
      }
      
      if (autoHScroll && hScroll != NULL) {
        hScroll->color.w = 1;
      }
    }
    
    bool Scroll::TouchTapIntent() {
      if (touchProc.IsAlive()) {
        touchProc.Acquiere(this);
        
        return true;
      }
      
      return false;
    }
    
    bool Scroll::TouchAcceptDrag() {
      touchProc.Acquiere(this);
      
      return true;
    }
    
    bool Scroll::TouchEvent(machete::input::Touch *touch) {
      Rect2D bounds = GetGlobalBounds();
      bool contained = bounds.Contains(touch->current);
      
      if (!touchProc.IsAlive()) {
        if (!touchProc.IsTracking() && !contained) {
          return false;
        }
      } else {
        if (contained) {
          if (touchProc.Gather(touch, bounds)) {
            return true;
          }
        }
      }
      
      if (touch->phase == machete::input::TouchStart && !contained) {
        return false;
      }
      
      if (container->TouchEvent(touch) == true) {
        return true;
      }
      
      return touchProc.Gather(touch, bounds);
    }
    
    void Scroll::CalculateElastic(float time) {
      elastic.x = 0;
      elastic.y = 0;
      
      bool procX = false;
      bool procY = false;
      
      if (container->position.x >= 0) {
        elastic.x = container->position.x;
        procX = true;
      }
      
      if (container->position.y >= 0) {
        elastic.y = container->position.y;
        procY = true;
      }
      
      if (procX && procY) {
        return;
      }
      
      Rect2D cbounds = container->GetBounds();
      
      if (procX == false) {
        float mx = cmin(0, bounds.size.x - cbounds.size.x);
        if (container->position.x < mx) {
          elastic.x = container->position.x - mx;
        }
      }
      
      if (procY == false) {
        float my = cmin(0, bounds.size.y - cbounds.size.y);
        if (container->position.y < my) {
          elastic.y = container->position.y - my;
        }
      }
    }
    
    void Scroll::ConfigureVScroll(bool allowVScroll, bool drawVScroll, bool autoVScroll) {
      this->allowVScroll = allowVScroll;
      this->drawVScroll = drawVScroll;
      this->autoVScroll = autoVScroll;
      allowDragY = allowVScroll;
    }
    
    void Scroll::ConfigureHScroll(bool allowHScroll, bool drawHScroll, bool autoHScroll) {
      this->allowHScroll = allowHScroll;
      this->drawHScroll = drawHScroll;
      this->autoHScroll = autoHScroll;
      allowDragX = allowHScroll;
    }
    
    void Scroll::SetFreeDrag(bool freeDrag) {
      this->freeDrag = freeDrag;
    }
    
    bool Scroll::IsFreeDrag() const {
      return freeDrag;
    }
    
    void Scroll::AddGluePoint(const Vec2 &gp) {
      gluePoints.Append(gp);
    }
    
    void Scroll::AddGluePoint(float x, float y) {
      Vec2 gp(x, y);
      AddGluePoint(gp);
    }
    
    void Scroll::CenterView(const Vec2 &cp) {
      if (cp.x != center.x || cp.y != center.y) {
        targetGlue = cp;
        centering = true;
        centered = false;
      }
    }
    
    void Scroll::SeekGluePoint(const Vec2 &dir) {
      bool still = false;
      
      Vec2 target = this->center;
      if (touchProc.IsAlive()) {
        target += (dir * -20.0f);
      }
      Vec2 tdiff = target - this->center;
      float targetAngle = 0;
      if (dir.x == 0 && dir.y == 0) {
        still = true;
      } else {
        targetAngle = catan2(tdiff.y, tdiff.x);
      }
      float closerLen = cmax(size.LengthSquared() * 0.01f - tdiff.LengthSquared(), 0);
      
      Vec2 current = center;
      Vec2 diff;
      Vec2 p;
      
      bool found = false;
      float len = size.LengthSquared();
      
      gluePoints.Reset();
      while (gluePoints.Next()) {
        p = gluePoints.GetCurrent()->GetValue();
        diff = p - this->center;
        float plen = diff.LengthSquared();
        
        float thisAngle = catan2(diff.y, diff.x);
        
        if (plen < closerLen) {
          if (plen < len || found == false) {
            found = true;
            current = p;
            len = plen;
          }
          continue;
        }
        
        if (still == false && cabs(targetAngle - thisAngle) > 0.2f) continue;

        if (plen < len || found == false) {
          found = true;
          current = p;
          len = plen;
        }
      }
      
      touchProc.Stop();
      if (found) {
        CenterView(current);
        return;
      }
      
      current = target;
      
      gluePoints.Reset();
      while (gluePoints.Next()) {
        p = gluePoints.GetCurrent()->GetValue();
        diff = p - this->center;
        float plen = diff.LengthSquared();
        
        if (!found || plen < len) {
          found = true;
          current = p;
          len = plen;
        }
      }
      
      CenterView(current);
    }
    
    void Scroll::StepTarget(float time) {
      if (!centering) return;
      
      Vec2 diff = targetGlue - center;

      Vec2 strength(-diff.x * SCROLL_ELASTICITY * time, -diff.y * SCROLL_ELASTICITY * time);

      if (diff.x > 0) {
        if (diff.x + strength.x < 0) {
          strength.x = -diff.x;
        }
      } else if (diff.x < 0) {
        if (diff.x + strength.x > 0) {
          strength.x = -diff.x;
        }
      }
      
      if (diff.y > 0) {
        if (diff.y + strength.y < 0) {
          strength.y = -diff.y;
        }
      } else if (diff.y < 0) {
        if (diff.y + strength.y > 0) {
          strength.y = -diff.y;
        }
      }
      
      ElasticMovement(strength);
    }
    
    Vec2 & Scroll::GetCenter() {
      return center;
    }
    
    bool Scroll::IsCentered() const {
      return centered;
    }
    
    bool Scroll::IsCentering() const {
      return centering;
    }

    TouchContainer::TouchContainer() : touchProc(this) {
      Container();
      
      event = NULL;
      
      
    }
    
    TouchContainer::~TouchContainer() {
      
    }

    bool TouchContainer::TouchAcceptTap() {
      return allowTap;
    }

    bool TouchContainer::TouchTapIntent() {
      return allowTap;
    }
    
    void TouchContainer::TouchTapPerformed() {
      if (event != NULL) {
        event->WidgetTapped(this);
      }
      
      touchProc.Release();
    }
    
    void TouchContainer::TouchTapCancelled() {
      touchProc.Release();
    }
    
    bool TouchContainer::TouchAcceptDragY() {
      return allowDragY;
    }

    bool TouchContainer::TouchAcceptDragX() {
      return allowDragX;
    }

    void TouchContainer::TouchDrag(machete::math::Vec2 &move) {
      if (event != NULL) {
        event->WidgetDragged(this, move);
      }
    }
    
    void TouchContainer::TouchInertia(machete::math::Vec2 &move) {
      if (event != NULL) {
        event->WidgetInertia(this, move);
      }
    }
    
    void TouchContainer::TouchEnded() {
      touchProc.Release();
    }
    
    bool TouchContainer::TouchEvent(machete::input::Touch *touch) {
      if (Container::TouchEvent(touch) == true) {
        return true;
      }
      
      Rect2D bounds = GetGlobalBounds();
      
      return touchProc.Gather(touch, bounds);
    }
    
    void TouchContainer::SetEventListener(machete::widget::WidgetEventAdapter *event) {
      this->event = event;
    }
    
    WidgetEventAdapter* TouchContainer::GetEventListener() {
      return event;
    }
    
    void TouchContainer::Update(float time) {
      touchProc.Update(time);
      
      Container::Update(time);
    }

  }
}
