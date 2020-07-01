#version 400 core
in vec3 position;
in vec3 colour;
in vec2 textureCoords;

out vec2 fragmentTextureCoords;

uniform mat4 viewProjection;

void main()
{
    // Pass relevant attributes to fragment shader
    fragmentTextureCoords = textureCoords;

    // Multiply projection by vertex position
    // MATRIX MULTIPLICATION IS NOT COMMUTATIVE
    // !!! PROJECTION * VIEW * MODEL !!!
    gl_Position = viewProjection * vec4(position, 1.0);
}