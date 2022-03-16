#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;
layout(location = 2) in vec3 offset;

out vec3 out_col;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * vec4(pos + offset, 1.0);
   out_col = col;
}                                        