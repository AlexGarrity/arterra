#version 400 core
out vec4 outColour;

in vec2 fragmentTextureCoords;

uniform sampler2D fragmentTexture;
uniform vec4 fragmentColour;

void main()
{
    outColour = texture(fragmentTexture, fragmentTextureCoords) * fragmentColour;
}