#version 400 core
layout(location = 0) in vec3 position;
<<<<<<< HEAD
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normalCoords;

out vec2 fragmentTextureCoords;
out vec3 fragmentNormalCoords;
=======
layout(location = 1) in vec3 colour;

out vec3 v_Colour;
>>>>>>> feature/renderer

uniform mat4 viewProjection;

void main()
{
<<<<<<< HEAD
    // Pass relevant attributes to fragment shader
    fragmentTextureCoords = textureCoords;
    fragmentNormalCoords = normalCoords;

    // Multiply projection by vertex position
    // MATRIX MULTIPLICATION IS NOT COMMUTATIVE
    // !!! PROJECTION * VIEW * MODEL !!!
=======
>>>>>>> feature/renderer
    gl_Position = viewProjection * vec4(position, 1.0);
    v_Colour = colour;
}