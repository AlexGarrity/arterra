#version 400 core
<<<<<<< HEAD
out vec4 outColour;

in vec2 fragmentTextureCoords;
in vec3 fragmentNormalCoords;

uniform sampler2D fragmentTexture;

void main()
{
    outColour = texture(fragmentTexture, fragmentTextureCoords);
=======

in vec3 v_Colour;

out vec4 colour;

void main()
{
    colour = vec4(v_Colour, 1.0);
>>>>>>> feature/renderer
}