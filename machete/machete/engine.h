//
//  IRenderingEngine.hpp
//  machete
//
//  Created by Hilario Perez Corona on 4/10/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#pragma once

#define STRINGIFY(A) #A

#include "vector.h"

struct Tex {
    unsigned int id;
    ivec2 size;
};

enum DeviceOrientation {
    DeviceOrientationUnknown,
    DeviceOrientationPortrait,
    DeviceOrientationPortraitUpsideDown,
    DeviceOrientationLandscapeLeft,
    DeviceOrientationLandscapeRight
};

struct IResourceManager {
    IResourceManager() {}
    virtual void LoadImage(const char* filename) = 0;
    virtual void* GetImageData() = 0;
    virtual ivec2 GetImageSize() = 0;
    virtual void UnloadImage() = 0;
    virtual ~IResourceManager() {};
};

struct IRenderingEngine* CreateRendereriOS1(IResourceManager* rm);
struct IRenderingEngine* CreateRendereriOS2(IResourceManager* rm);
struct IRenderingEngine* CreateRendererAndroid1(IResourceManager* rm);
struct IRenderingEngine* CreateRendererAndroid2(IResourceManager* rm);

struct IRenderingEngine {
    virtual void Initialize(int width, int height) = 0;
    virtual ~IRenderingEngine() {}
    virtual void Draw(vec2 pos, vec2 size, float rotation, float scale, vertuv* verts, unsigned int texId) = 0;
    virtual void Draw() = 0;
    virtual unsigned int CreateBuffer(vertuv* verts) const = 0;
    virtual void DeleteBuffer(unsigned int buffer) const = 0;
    virtual Tex CreateTexture(const char* texture) = 0;
    virtual void Clear() = 0;
    virtual ivec2 GetScreenSize() const = 0;
};

struct IRenderable {
	virtual void Draw(vec2 pos, float rotation, float scale, vec4 color, IRenderingEngine* engine) = 0;
	virtual Rect2D GetBounds() = 0;
};

struct IGame {
	virtual void Initialize(int, int, DeviceOrientation) = 0;
	virtual void Resize(int, int, DeviceOrientation) = 0;
	virtual void Update(float time) = 0;
	virtual void Render() = 0;
	virtual void OnPause() = 0;
	virtual void OnResume() = 0;
	virtual void OnStart() = 0;
	virtual void OnStop() = 0;
};

struct IGame* CreateGame(IRenderingEngine* rm);
