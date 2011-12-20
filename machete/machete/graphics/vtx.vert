
const char* VtxVertexShader = STRINGIFY(

uniform mat4 Projection;
uniform mat4 Base;
uniform mat4 Modelview;

attribute vec4 Pivot;
attribute vec4 Offset;
attribute vec4 Position;
attribute vec4 SourceColor;
attribute vec2 TextureCoord;
attribute vec2 Scale;
attribute float Rotation;

varying vec2 TextureCoordOut;

void main(void) {
  float radians = Rotation * 3.14159 / 180.0;
  float s = sin(radians);
  float c = cos(radians);
  
  vec4 Pivot2 = vec4(Pivot.x, -Pivot.y, 0, 0);
  vec4 Offset2 = vec4(Offset.x, -Offset.y, 0, 0);
  
  mat4 scale = mat4(Scale.x, 0, 0, 0,
                    0, Scale.y, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
  
  mat4 rot = mat4(c, s, 0, 0,
                  -s, c, 0, 0,
                  0, 0, 1, 0,
                  0, 0, 0, 1);
  
  mat4 alloc1 = mat4(1, 0, 0, 0,
                     0, 1, 0, 0,
                     0, 0, 1, 0,
                     Pivot2.x, Pivot2.y, 0, 1);
  
  mat4 alloc2 = mat4(1, 0, 0, 0,
                     0, 1, 0, 0,
                     0, 0, 1, 0,
                     0, 0, 0, 1);
  
  mat4 Transform = alloc2 * rot * scale * alloc1;
  
  vec4 NewPos = Position;
  
  vec4 Pos = Transform * NewPos;
  
  Pos += Offset2;
  
  gl_Position = Projection * Base * Modelview * Pos;
  
  TextureCoordOut.x = TextureCoord.x;
	TextureCoordOut.y = TextureCoord.y;
}

);
