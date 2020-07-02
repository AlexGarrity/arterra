#version 400 core
out vec4 outColour;

in vec2 fragmentTextureCoords;
in vec3 fragmentNormalCoords;

uniform sampler2D fragmentTexture;

void main()
{
    outColour = texture(fragmentTexture, fragmentTextureCoords);
}