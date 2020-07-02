#version 400 core

in vec3 v_Colour;

out vec4 colour;

void main()
{
    colour = vec4(v_Colour, 1.0);
}