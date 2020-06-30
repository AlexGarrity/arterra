#version 150 core
in vec3 position;
in vec3 color;
out vec3 Color;

uniform mat4 viewProjection;

void main()
{
    Color = color;
    gl_Position = vec4(position, 1.0); //* viewProjection;
}