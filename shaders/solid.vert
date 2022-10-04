#version 330
// in_Position was bound to attribute index 0 and in_Color was bound to attribute index 1
in vec2 position;
in vec3 color;

// We output the ex_Color variable to the next shader in the chain
out vec3 InterpolatedColor;
void main(void) {
    // Since we are using flat lines, our input only had two points: x and y.
    // Set the Z coordinate to 0 and W coordinate to 1

    float pi = 3.14159;

    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
    // gl_Position = vec4(sin(2*pi*gl_VertexID), cos(2*pi*gl_VertexID), 0.0, 1.0);
    gl_PointSize = 3.3;

    // GLSL allows shorthand use of vectors too, the following is also valid:
    // gl_Position = vec4(in_Position, 0.0, 1.0);

    // We're simply passing the color through unmodified
    InterpolatedColor = color;
    // InterpolatedColor = vec3(0.2,0.3,0.2);
}