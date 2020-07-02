#version 400 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normalCoords;

out vec2 fragmentTextureCoords;
out vec3 fragmentNormalCoords;

uniform mat4 viewProjection;

void main()
{
    // Pass relevant attributes to fragment shader
    
    fragmentTextureCoords = textureCoords;
    fragmentNormalCoords = normalCoords;

    // Multiply projection by vertex position
    // MATRIX MULTIPLICATION IS NOT COMMUTATIVE
    // !!! PROJECTION * VIEW * MODEL !!!
    gl_Position = viewProjection * vec4(position, 1.0);
}