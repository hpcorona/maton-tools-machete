
const char* VtxFragmentShader = STRINGIFY(

uniform lowp vec4 DestinationColor;
varying highp vec2 TextureCoordOut;

uniform sampler2D Sampler;

void main(void) {
  gl_FragColor = texture2D(Sampler, TextureCoordOut) * DestinationColor;
}

);
