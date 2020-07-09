#version 400 core
out vec4 outColour;

// in vec2 fragmentTextureCoords;
// in vec3 fragmentNormalCoords;
in vec3 fragmentPosition;

uniform sampler2D fragmentTexture;
uniform vec4 fragmentColour;

void main()
{
    // outColour = texture(fragmentTexture, fragmentTextureCoords) * fragmentColour;
    outColour = vec4(mod(fragmentPosition, 16.0f) / 16.0f, 1.0f);
}