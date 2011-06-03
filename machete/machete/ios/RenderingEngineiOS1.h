//
//  RenderingEngineiOS1.h
//  machete
//
//  Created by Hilario Perez Corona on 4/10/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "../engine.h"

#ifdef TARGET_IOS

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include "../quaternion.h"

#define MAX_RING 10
#define MAX_SPR 300
#define MAX_VTX (MAX_SPR*4)
#define MAX_IDX (MAX_SPR*6)

namespace machete {

class RenderingEngineiOS1 : public IRenderingEngine {
public:
  RenderingEngineiOS1(IResourceManager* rm);
  void Initialize(int width, int height);
  void Draw(vec2 pos, vec2 size, float rotation, float scale, vertuv* verts, unsigned int texId);
  void Draw();
    
  GLuint CreateBuffer(vertuv* verts) const;
  void DeleteBuffer(GLuint buffer) const;
  void CreateBuffers();

  Tex CreateTexture(const char* texture);
  void Clear();
  ivec2 GetScreenSize() const;
  
  IResourceManager* GetResMan() const { return resMan; }
    
private:
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
    
  ivec2 size;
  GLuint lastBind;
  GLuint vtxBind;
  GLuint batchBind;
};

machete::IRenderingEngine* CreateRendereriOS1(machete::IResourceManager* rm) {
    return new machete::RenderingEngineiOS1(rm);
}

#else
machete::IRenderingEngine* CreateRendereriOS1(machete::IResourceManager* rm) {
    return NULL;
}
#endif

}
