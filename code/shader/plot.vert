#version 450 core

attribute vec2 position;

uniform float LEN;
uniform vec2 OFF;

void main(void)
{
    gl_Position = (vec4(0.0, 0.0, 0.0, 0.0) + vec4(position.x, position.y, 0.0,  1.0));
}
