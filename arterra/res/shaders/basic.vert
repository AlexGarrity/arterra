#version 400 core
layout(location = 0) in vec3 position;
// layout(location = 1) in vec2 textureCoords;
// layout(location = 2) in vec3 normalCoords;

// out vec2 fragmentTextureCoords;
// out vec3 fragmentNormalCoords;
out vec3 fragmentPosition;

uniform mat4 viewProjection;
// uniform mat4 modelPos;

void main()
{
    // Pass relevant attributes to fragment shader
    // fragmentTextureCoords = textureCoords;
    // fragmentNormalCoords = normalCoords;
    
    fragmentPosition = position;

    // Multiply projection by vertex position
    // MATRIX MULTIPLICATION IS NOT COMMUTATIVE
    // !!! PROJECTION * VIEW * MODEL !!!
    gl_Position = viewProjection * vec4(position, 1.0);
}