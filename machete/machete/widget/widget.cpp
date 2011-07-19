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

      // Top Left UV
      verts[0].uv.x = uv0.x; verts[0].uv.y = uv0.y;
      verts[1].uv.x = uv0.x; verts[1].uv.y = uvi0.y;
      verts[2].uv.x = uvi0.x; verts[2].uv.y = uv0.y;
      verts[3].uv.x = uvi0.x; verts[3].uv.y = uvi0.y;

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

      // Bottom Right UV
      verts[12].uv.x = uvi1.x; verts[12].uv.y = uvi1.y;
      verts[13].uv.x = uvi1.x; verts[13].uv.y = uv1.y;
      verts[14].uv.x = uv1.x; verts[14].uv.y = uvi1.y;
      verts[15].uv.x = uv1.x; verts[15].uv.y = uv1.y;
      
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
    }
    
    void MetaWidget::Draw(DrawContext *ctx, const Vec2 & pivot, const Vec2 & pos, const Vec2 & scale, const Vec4 & color, float rotation, bool flipX, bool flipY) {
      
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
    }
    
    Widget::Widget(const Vec2 & size) : touchProc(this) {
      this->size = size;
      state = NULL;
      event = NULL;
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
        
        if (changed) {
          state->Draw(ctx, ZERO2, ZERO2, ONE2, NewColor, 0, false, false);
        } else {
          state->Draw(ctx, ZERO2, Position, scale, NewColor, rotation, false, false);
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
    
    void Widget::TouchTapIntent() {
      
    }
    
    void Widget::TouchTapCancelled() {
      
    }
    
    void Widget::TouchTapPerformed() {
      if (event != NULL) {
        event->WidgetTapped(this);
      }
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
    
    void Widget::SetEventListener(WidgetEventAdapter *event) {
      this->event = event;
    }
    
    WidgetEventAdapter *Widget::GetEventListener() {
      return event;
    }
    
    Button::Button() {
      font = NULL;
      
      label = new Text(30);
      
      Container::Add(label);
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
    
    void Button::TouchTapIntent() {
      SetState("pressed");
      
      Widget::TouchTapIntent();
    }
    
    void Button::TouchTapCancelled() {
      SetState("normal");

      Widget::TouchTapCancelled();
    }
    
    void Button::TouchTapPerformed() {
      SetState("normal");
      
      Widget::TouchTapPerformed();
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
    
  }
}
