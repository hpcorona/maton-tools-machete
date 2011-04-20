//
//  RenderingEngineiOS1.cpp
//  machete
//
//  Created by Hilario Perez Corona on 4/10/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "RenderingEngineAndroid1.h"

#ifdef TARGET_ANDROID

RenderingEngineAndroid1::RenderingEngineAndroid1(IResourceManager* sm) {
    resMan = sm;
}

ivec2 RenderingEngineAndroid1::GetScreenSize() const { return size; }

void RenderingEngineAndroid1::Initialize(int width, int height) {
    lastBind = -1;
    
    size.x = width;
    size.y = height;
    
    glDisable(GL_DITHER);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glOrthof(0, width, 0, height, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, size.y, 0);

    glClearColor(0.5f, 0.5f, 0.5f, 1);
    
    CreateBuffers();
}

void RenderingEngineAndroid1::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderingEngineAndroid1::Draw(vec2 pos, vec2 size, float rotation, float scale, vertuv *verts, unsigned int texId) {
    glPushMatrix();

	glTranslatef(pos.x, pos.y, 0);
	glTranslatef(size.x / 2, -size.y / 2, 0);
	glRotatef(rotation, 0, 0, 1);
	glScalef(scale, scale, scale);
	glTranslatef(-size.x / 2, size.y / 2, 0);

    glVertexPointer(3, GL_FLOAT, sizeof(vertuv), &verts[0].vert);
    glTexCoordPointer(2, GL_FLOAT, sizeof(vertuv), &verts[0].uv);
    glColorPointer(4, GL_FLOAT, sizeof(vertuv), &verts[0].color);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);

	glPopMatrix();
}

void RenderingEngineAndroid1::Draw() {
}

GLuint RenderingEngineAndroid1::CreateBuffer(vertuv* verts) const {
    GLuint buff;

    glGenBuffers(1, &buff);
    glBindBuffer(GL_ARRAY_BUFFER, buff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertuv) * MAX_VTX, verts, GL_DYNAMIC_DRAW);

    return buff;
}

void RenderingEngineAndroid1::DeleteBuffer(GLuint buffer) const {
    glDeleteBuffers(1, &buffer);
}


Tex RenderingEngineAndroid1::CreateTexture(const char* texture) {
    Tex tTex;
    
    glGenTextures(1, &tTex.id);
    glBindTexture(GL_TEXTURE_2D, tTex.id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                       GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                       GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                       GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                       GL_CLAMP_TO_EDGE);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
                 GL_MODULATE);
    
    resMan->LoadImage(texture);
    void* pixels = resMan->GetImageData();
    ivec2 size = resMan->GetImageSize();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    resMan->UnloadImage();
    
    return tTex;
}

void RenderingEngineAndroid1::CreateBuffers() {
    unsigned int vtx = 0;
    
    for (unsigned int idx = 0; idx < MAX_IDX; idx += 6) {
        indices[idx] = vtx; indices[idx+1] = vtx+1; indices[idx+2] = vtx+2;
        indices[idx+3] = vtx+2; indices[idx+4] = vtx+1; indices[idx+5] = vtx+3;
        
        vtx += 4;
    }
    
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * MAX_IDX, indices, GL_STATIC_DRAW);
}

#endif
