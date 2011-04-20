//
//  MAModule.h
//  machete
//
//  Created by Hilario Perez Corona on 4/10/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//
#include "vector.h"
#include "engine.h"

struct module : public IRenderable {
    vec2 size;
    vertuv verts[4];
    unsigned int texId;
    
    module() { }

    module(vec2 s, vec2 uv0, vec2 uv1, unsigned int tid) {
        size = s;
        
        verts[0].vert.x = 0; verts[0].vert.y = 0; verts[0].vert.z = 0;
        verts[1].vert.x = 0; verts[1].vert.y = -s.y; verts[1].vert.z = 0;
        verts[2].vert.x = s.x; verts[2].vert.y = 0; verts[2].vert.z = 0;
        verts[3].vert.x = s.x; verts[3].vert.y = -s.y; verts[3].vert.z = 0;
        
#ifdef TARGET_ANDROID
        verts[0].uv.x = uv0.x; verts[0].uv.y = uv1.y;
        verts[1].uv.x = uv0.x; verts[1].uv.y = uv0.y;
        verts[2].uv.x = uv1.x; verts[2].uv.y = uv1.y;
        verts[3].uv.x = uv1.x; verts[3].uv.y = uv0.y;
#else
        verts[0].uv.x = uv0.x; verts[0].uv.y = uv0.y;
        verts[1].uv.x = uv0.x; verts[1].uv.y = uv1.y;
        verts[2].uv.x = uv1.x; verts[2].uv.y = uv0.y;
        verts[3].uv.x = uv1.x; verts[3].uv.y = uv1.y;
#endif
        
        verts[0].mra.x = s.x / 2; verts[0].mra.y = s.y / 2;
        verts[1].mra.x = s.x / 2; verts[1].mra.y = s.y / 2;
        verts[2].mra.x = s.x / 2; verts[2].mra.y = s.y / 2;
        verts[3].mra.x = s.x / 2; verts[3].mra.y = s.y / 2;
        
        texId = tid;
    }
    
    void Draw(vec2 pos, float rotation, float scale, vec4 color, IRenderingEngine* engine) {
        verts[0].color = color;
        verts[1].color = color;
        verts[2].color = color;
        verts[3].color = color;

        verts[0].mra.z = rotation; verts[0].mra.w = scale;
        verts[1].mra.z = rotation; verts[1].mra.w = scale;
        verts[2].mra.z = rotation; verts[2].mra.w = scale;
        verts[3].mra.z = rotation; verts[3].mra.w = scale;
        
        verts[0].uv.z = pos.x; verts[0].uv.w = -pos.y;
        verts[1].uv.z = pos.x; verts[1].uv.w = -pos.y;
        verts[2].uv.z = pos.x; verts[2].uv.w = -pos.y;
        verts[3].uv.z = pos.x; verts[3].uv.w = -pos.y;

        engine->Draw(vec2(pos.x, -pos.y), size, rotation, scale, verts, texId);
    }
    
    Rect2D GetBounds() {
        return Rect2D(vec2(0, 0), size);
    }
};

