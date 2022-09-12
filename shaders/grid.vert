#version 330

uniform float time;
uniform float psize;
uniform float width;

out vec4 InterpolatedColor;

void main(){
  //float width = 10.0f;

  float x = mod(gl_VertexID, width);
  float y = floor(gl_VertexID / width);

  float u = x / (width - 1.0f);
  float v = y / (width - 1.0f);

  float xOffset = cos(time + y * 0.2f) * 0.1f;
  float yOffset = cos(time + x * 0.3f) * 0.2f;

  float ux = u * 2.0f - 1.0f + xOffset;
  float uy = v * 2.0f - 1.0f + yOffset;

  vec2 xy = vec2(ux, uy) * 1.2f;

  gl_Position = vec4(xy, 0.0f, 1.0f);
  gl_PointSize = psize;
  InterpolatedColor = vec4(sin(time*x), cos(time*y), cos(x*y), 1.0);
}