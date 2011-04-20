//
//  RenderingEngineiOS1.h
//  machete
//
//  Created by Hilario Perez Corona on 4/10/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "../engine.h"

#ifdef TARGET_ANDROID

#include <GLES/gl.h>
#include <GLES/glext.h>
#include "../quaternion.h"

#define MAX_RING 1
#define MAX_SPR 1
#define MAX_VTX (MAX_SPR*4)
#define MAX_IDX (MAX_SPR*6)

class RenderingEngineAndroid1 : public IRenderingEngine {
public:
	RenderingEngineAndroid1(IResourceManager* rm);
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
    IResourceManager* resMan;
    
    GLushort indices[MAX_IDX];
    vertuv* vertexes;
    GLuint indexBuffer;
    
    ivec2 size;
    GLuint lastBind;
};

IRenderingEngine* CreateRendererAndroid1(IResourceManager* rm) {
    return new RenderingEngineAndroid1(rm);
}

#else
IRenderingEngine* CreateRendererAndroid1(IResourceManager* rm) {
    return NULL;
}
#endif
