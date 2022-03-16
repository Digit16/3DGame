#version 330 core

layout(location = 0) out vec4 color;
in vec3 out_col;

void main()
{
   color = vec4(out_col, 1.0);
}                                      