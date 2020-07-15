#version 400 core
layout(location = 0) in vec2 i_position;
layout(location = 1) in vec2 i_textureCoords;

uniform mat4 viewProjection;

out vData
{
    vec2 textureCoords;
}vertex;

void main()
{
    // Pass relevant attributes to fragment shader
    vertex.textureCoords = i_textureCoords;

    gl_Position = viewProjection * vec4(i_position, -1.0, 1.0);
}