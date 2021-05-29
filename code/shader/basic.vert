#version 450 core

attribute vec2 position;

uniform mat4 ROT;
uniform float LEN;
uniform vec2 OFF;
void main(void)
{
    gl_Position = (vec4(0.0, 1.0, 0.0, 0.0) + (ROT) * (-vec4(LEN*OFF,0.0, 0.0) + vec4(LEN*position.x, LEN*position.y, 0.0, 1.0)));
}
