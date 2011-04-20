//
//  RenderingEngineiOS2.h
//  machete
//
//  Created by Hilario Perez Corona on 4/10/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "../engine.h"

#ifdef TARGET_IOS

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <cmath>
#include <iostream>
#include "../quaternion.h"

#include "../simple.vert"
#include "../simple.frag"

#define MAX_RING 10
#define MAX_SPR 300
#define MAX_VTX (MAX_SPR*4)
#define MAX_IDX (MAX_SPR*6)

class RenderingEngineiOS2 : public IRenderingEngine {
public:
    RenderingEngineiOS2(IResourceManager* rm);
    GLuint BuildShader(const char* source, GLenum shaderType) const;
    GLuint BuildProgram(const char* vShader, const char* fShader) const;
    void ApplyOrtho(float maxX, float maxY) const;
    void Initialize(int width, int height);
    void Draw(vec2 pos, vec2 size, float rotation, float scale, vertuv* verts, unsigned int texId);
    void Draw();
    GLuint CreateBuffer(vertuv* verts) const;
    void DeleteBuffer(GLuint buffer) const;
    void CreateBuffers();

    Tex CreateTexture(const char* texture);
    void Clear();
    ivec2 GetScreenSize() const;

private:
    GLuint simpleProgram;
    GLuint framebuffer;
    GLuint renderbuffer;
    IResourceManager* resMan;

    GLushort indices[MAX_IDX];
    vertuv vertexesRing[MAX_RING][MAX_VTX];
    vertuv* vertexes;
    GLuint indexBuffer;
    GLuint vertexBuffer;
    GLuint vertexBufferRing[MAX_RING];
    unsigned int ringIdx;
    unsigned int primCount;
    unsigned int vtxCount;

    GLuint positionSlot;
    GLuint colorSlot;
    GLuint samplerSlot;
    GLuint textureSlot;
    GLuint midRotScaleSlot;
    GLuint modelviewSlot;
    
    ivec2 size;
    mat4 base;
    GLuint lastBind;
    GLuint vtxBind;
};

IRenderingEngine* CreateRendereriOS2(IResourceManager *rm) {
    return new RenderingEngineiOS2(rm);
}

#else
IRenderingEngine* CreateRendereriOS2(IResourceManager *rm) {
    return NULL;
}
#endif
