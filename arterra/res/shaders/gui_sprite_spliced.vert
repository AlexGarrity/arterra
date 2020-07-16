#version 400 core
layout(location = 0) in vec2 i_position;
layout(location = 1) in vec2 i_textureCoords;

uniform mat4 u_ViewProjection;

// Data passed to the fragment shader
out vData
{
    vec2 textureCoords;
}vertex;

void main()
{
    // Pass relevant data to fragment shader
    vertex.textureCoords = i_textureCoords;
    
    // Apply the projection to the coordinates
    gl_Position = u_ViewProjection * vec4(i_position, 0.0, 1.0);
}