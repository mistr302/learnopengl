#version 460 core
layout(location = 0) in vec2 aPos; // input from vertex buffer
out vec2 uv;                       // output to fragment shader

void main() {
    uv = aPos;                      // pass position to fragment shader
    gl_Position = vec4(aPos, 0.0, 1.0);
}
