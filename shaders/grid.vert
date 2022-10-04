#version 330
uniform float time;
uniform float psize;
uniform float width;
uniform float py;
out vec4 InterpolatedColor;
void main(){
    //float width = 10.0f;
    float x = float(gl_VertexID)/ width;
    float y = float(gl_VertexID)/ width;
    float u = x / (width - 1.0f);
    float v = y / (width - 1.0f);
    float xOffset = cos(time + y * 0.2f) * 0.1f;
    float yOffset = cos(time + x * 0.3f) * 0.2f;
    float ux = u * 2.0f - 1.0f + xOffset;
    float uy = v * 2.0f - 1.0f + yOffset;
    vec2 z = vec2(sin(time/ux*uy+1.33), tan(time +ux*uy));
    // vec2 xy = vec2(ux, uy) * 1.2f;
    vec2 xy = vec2(ux, uy) + z;
    float r = sin(time*x);
    float g = cos(time*y);
    float b = cos(x*y);
    // gl_Position = vec4(xy*b, r, g);
    gl_Position = vec4(xy, 0.0f, abs(sin(py+time/4)));
    gl_PointSize = psize;
    InterpolatedColor = vec4(r, g, b, 1.0);
}
