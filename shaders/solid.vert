#version 330
uniform float time;

out vec4 InterpolatedColor;
void main(){
    float r = sin(time);
    float g = cos(time);
    float b = sin(time) * cos(time);
    gl_Position = vec4(r,g,b, 1.0);
    InterpolatedColor = vec4(r, g, b, 1.0);
}
