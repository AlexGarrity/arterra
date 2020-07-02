#version 400 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;

out vec3 v_Colour;

uniform mat4 viewProjection;

void main()
{
    gl_Position = viewProjection * vec4(position, 1.0);
    v_Colour = colour;
}