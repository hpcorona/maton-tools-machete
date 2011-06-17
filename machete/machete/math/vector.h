//
//  vector.h
//  machete
//
//  Created by Hilario Perez Corona on 4/11/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//
#pragma once

#ifdef TARGET_IOS

#include <math.h>

#define cabs(a) fabs(a)
#define catan(a) atan(a)
#define cmin(a,b) (a>b?b:a)
#define cmax(a,b) (a>b?a:b)

#else

#include <math.h>
#include <GLES/gl.h>
#define cmin(a,b) fmin(a,b)
#define cmax(a,b) fmax(a,b)
#define cabs(a) fabs(a)

#endif

namespace machete { 
  namespace math {
    
    const float Pi = 4 * catan(1.0f);
    const float TwoPi = 2 * Pi;
    
    template <typename T>
    struct Vector2 {
      Vector2() {}
      Vector2(T x, T y) : x(x), y(y) {}
      T Dot(const Vector2& v) const
      {
        return x * v.x + y * v.y;
      }
      Vector2 operator+(const Vector2& v) const
      {
        return Vector2(x + v.x, y + v.y);
      }
      Vector2 operator-(const Vector2& v) const
      {
        return Vector2(x - v.x, y - v.y);
      }
      void operator+=(const Vector2& v)
      {
        *this = Vector2(x + v.x, y + v.y);
      }
      void operator-=(const Vector2& v)
      {
        *this = Vector2(x - v.x, y - v.y);
      }
      Vector2 operator/(float s) const
      {
        return Vector2(x / s, y / s);
      }
      Vector2 operator*(float s) const
      {
        return Vector2(x * s, y * s);
      }
      void operator/=(float s)
      {
        *this = Vector2(x / s, y / s);
      }
      void operator*=(float s)
      {
        *this = Vector2(x * s, y * s);
      }
      void Normalize()
      {
        float s = 1.0f / Length();
        x *= s;
        y *= s;
      }
      Vector2 Normalized() const 
      {
        Vector2 v = *this;
        v.Normalize();
        return v;
      }
      T LengthSquared() const
      {
        return x * x + y * y;
      }
      T Length() const
      {
        return sqrt(LengthSquared());
      }
      const T* Pointer() const
      {
        return &x;
      }
      operator Vector2<float>() const
      {
        return Vector2<float>(x, y);
      }
      bool operator==(const Vector2& v) const
      {
        return x == v.x && y == v.y;
      }
      Vector2 Lerp(float t, const Vector2& v) const
      {
        return Vector2(x * (1 - t) + v.x * t,
                       y * (1 - t) + v.y * t);
      }
      template <typename P>
      P* Write(P* pData)
      {
        Vector2* pVector = (Vector2*) pData;
        *pVector++ = *this;
        return (P*) pVector;
      }
      T x;
      T y;
    };
    
    template <typename T>
    struct Vector3 {
      Vector3() {}
      Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
      T Length()
      {
        return sqrt(x * x + y * y + z * z);
      }
      void Normalize()
      {
        float s = 1.0f / Length();
        x *= s;
        y *= s;
        z *= s;
      }
      Vector3 Normalized() const 
      {
        Vector3 v = *this;
        v.Normalize();
        return v;
      }
      Vector3 Cross(const Vector3& v) const
      {
        return Vector3(y * v.z - z * v.y,
                       z * v.x - x * v.z,
                       x * v.y - y * v.x);
      }
      T Dot(const Vector3& v) const
      {
        return x * v.x + y * v.y + z * v.z;
      }
      Vector3 operator+(const Vector3& v) const
      {
        return Vector3(x + v.x, y + v.y,  z + v.z);
      }
      void operator+=(const Vector3& v)
      {
        x += v.x;
        y += v.y;
        z += v.z;
      }
      void operator-=(const Vector3& v)
      {
        x -= v.x;
        y -= v.y;
        z -= v.z;
      }
      void operator/=(T s)
      {
        x /= s;
        y /= s;
        z /= s;
      }
      Vector3 operator-(const Vector3& v) const
      {
        return Vector3(x - v.x, y - v.y,  z - v.z);
      }
      Vector3 operator-() const
      {
        return Vector3(-x, -y, -z);
      }
      Vector3 operator*(T s) const
      {
        return Vector3(x * s, y * s, z * s);
      }
      Vector3 operator/(T s) const
      {
        return Vector3(x / s, y / s, z / s);
      }
      bool operator==(const Vector3& v) const
      {
        return x == v.x && y == v.y && z == v.z;
      }
      Vector3 Lerp(float t, const Vector3& v) const
      {
        return Vector3(x * (1 - t) + v.x * t,
                       y * (1 - t) + v.y * t,
                       z * (1 - t) + v.z * t);
      }
      const T* Pointer() const
      {
        return &x;
      }
      template <typename P>
      P* Write(P* pData)
      {
        Vector3<T>* pVector = (Vector3<T>*) pData;
        *pVector++ = *this;
        return (P*) pVector;
      }
      T x;
      T y;
      T z;
    };
    
    template <typename T>
    struct Vector4 {
      Vector4() {}
      Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
      Vector4(const Vector3<T>& v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}
      T Dot(const Vector4& v) const
      {
        return x * v.x + y * v.y + z * v.z + w * v.w;
      }
      Vector4 Lerp(float t, const Vector4& v) const
      {
        return Vector4(x * (1 - t) + v.x * t,
                       y * (1 - t) + v.y * t,
                       z * (1 - t) + v.z * t,
                       w * (1 - t) + v.w * t);
      }
      Vector4 operator+(const Vector4& v) const
      {
        return Vector4(x + v.x, y + v.y,  z + v.z, w + v.w);
      }
      void operator+=(const Vector4& v)
      {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
      }
      void operator-=(const Vector4& v)
      {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
      }
      
      const T* Pointer() const
      {
        return &x;
      }
      T x;
      T y;
      T z;
      T w;
    };
    
    typedef Vector2<bool> BVec2;
    
    typedef Vector2<int> IVec2;
    typedef Vector3<int> IVec3;
    typedef Vector4<int> IVec4;
    
    typedef Vector2<float> Vec2;
    typedef Vector3<float> Vec3;
    typedef Vector4<float> Vec4;
    
    struct Rect2D {
      Rect2D() {
        Clear();
      }
      
      Rect2D(float x, float y, float w, float h) {
        pos.x = x;
        pos.y = y;
        size.x = w;
        size.y = h;
      }
      
      Rect2D(Vec2 p, Vec2 s) {
        pos = p;
        size = s;
      }
      
      bool Empty() {
        return pos.x == 0 && pos.y == 0 && 
        size.x == 0 && size.y == 0;
      }
      
      void Clear() {
        pos.x = 0; pos.y = 0;
        size.x = 0; size.y = 0;
      }
      
      Rect2D operator+(const Rect2D& r) const {
        float x0 = cmin(pos.x, r.pos.x);
        float y0 = cmin(pos.y, r.pos.y);
        float x1 = cmax(pos.x + size.x, r.pos.x + r.size.x);
        float y1 = cmax(pos.y + size.y, r.pos.y + r.size.y);
        
        return Rect2D(x0, y0, x1 - x0, y1 - y0);
      }
      
      void operator+=(const Rect2D& r) {
        float x0 = cmin(pos.x, r.pos.x);
        float y0 = cmin(pos.y, r.pos.y);
        float x1 = cmax(pos.x + size.x, r.pos.x + r.size.x);
        float y1 = cmax(pos.y + size.y, r.pos.y + r.size.y);
        
        pos.x = x0;
        pos.y = y0;
        size.x = x1 - x0;
        size.y = y1 - y0;
      }
      
      Rect2D operator*(float scale) const {
        float x0 = pos.x * scale;
        float y0 = pos.y * scale;
        float x1 = (pos.x + size.x) * scale;
        float y1 = (pos.y + size.y) * scale;
        
        return Rect2D(x0, y0, x1 - x0, y1 - y0);
      }
      
      void operator*=(float scale) {
        float x0 = pos.x * scale;
        float y0 = pos.y * scale;
        float x1 = (pos.x + size.x) * scale;
        float y1 = (pos.y + size.y) * scale;
        
        pos.x = x0;
        pos.y = y0;
        size.x = x1 - x0;
        size.y = y1 - y0;
      }
      
      bool Contains(Vec2 point) const {
        return (point.x >= pos.x && point.y >= pos.y &&
                pos.x <= pos.x + size.x - 1 &&
                pos.y <= pos.y + size.y - 1);
      }
      
      Vec2 Center() const {
        return Vec2(pos.x + size.x / 2, pos.y + size.y / 2);
      }
      
      bool Intersects(const Rect2D& r) const {
        Vec2 m1 = Center();
        Vec2 m2 = r.Center();
        
        Vec2 d = m1 - m2;
        
        Vec2 w = size / 2 + r.size / 2;
        
        return (cabs(d.x) <= w.x &&
                cabs(d.y) <= w.y);
      }
      
      Vec2 pos;
      Vec2 size;
    };
    
  } 
}
