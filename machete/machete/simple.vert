
const char* SimpleVertexShader = STRINGIFY(

uniform mat4 Projection;
uniform mat4 Modelview;

attribute vec4 Position;
attribute vec4 SourceColor;
attribute vec4 TextureCoord;
attribute vec4 MidRotScale;

varying vec4 DestinationColor;
varying vec2 TextureCoordOut;
                                           
void main(void) {
    float radians = MidRotScale.z * 3.14159 / 180.0;
    float s = sin(radians);
    float c = cos(radians);

    vec4 fpos = vec4(TextureCoord.z, TextureCoord.w, 0, 0);
    
    mat4 identity = mat4(1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1);

    mat4 scale = mat4(MidRotScale.w, 0, 0, 0,
                      0, MidRotScale.w, 0, 0,
                      0, 0, MidRotScale.w, 0,
                      0, 0, 0, 1);
    
    mat4 rot = mat4(c, s, 0, 0,
                    -s, c, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
    
    mat4 alloc1 = mat4(1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       -MidRotScale.x - fpos.x, MidRotScale.y - fpos.y, 0, 1);

    mat4 alloc2 = mat4(1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       MidRotScale.x + fpos.x, -MidRotScale.y + fpos.y, 0, 1);
    
    mat4 result = alloc2 * rot * scale * alloc1;
    
    DestinationColor = SourceColor;
    
    vec4 np = Position + fpos;
    
    vec4 np2 = result * np;
    
    gl_Position = Projection * Modelview * np2;
    
    TextureCoordOut.x = TextureCoord.x; TextureCoordOut.y = TextureCoord.y;
}
                                           
);
