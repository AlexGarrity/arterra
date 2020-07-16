#version 400 core
in vData
{
    vec2 textureCoords;
};

uniform sampler2D fragmentTexture;
uniform vec4 u_ColourTint;

out vec4 o_Colour;

void main()
{
    o_Colour = texture(fragmentTexture, textureCoords);
    o_Colour *= u_ColourTint;
}