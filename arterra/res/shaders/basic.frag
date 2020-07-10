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
    float darkness = pow(fragmentPosition.y, 4) / 16777216.0f;
    outColour = vec4(
        0.013f, 
        mod(fragmentPosition.y, 1.0f) * darkness, 
        0.13f,
        1.0f
    );
}