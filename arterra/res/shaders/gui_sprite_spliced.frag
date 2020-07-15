#version 400 core
in vData
{
    vec2 textureCoords;
};

out vec4 o_colour;

uniform sampler2D fragmentTexture;
uniform vec4 colourTint;
uniform vec2 u_Pixelborder;
uniform float u_Textureborder;


float isInBorder(float coord, float pixelBorder) {
    if (coord < pixelBorder) {
        float percent = 1 - ((pixelBorder - coord) / pixelBorder);
        return u_Textureborder * percent;
    }else if (coord > (1-pixelBorder)) {
        float percent = 1 - ((pixelBorder - (1-coord)) / pixelBorder);
        return u_Textureborder * percent;
    }else {
        return coord;
    }
}

void main(void) {
    
    float x = isInBorder(textureCoords.x, u_Pixelborder.x);
    float y = isInBorder(textureCoords.y, u_Pixelborder.y);
    vec2 newUV = vec2(x, y);
    
    o_colour = texture(fragmentTexture, newUV);
    //o_colour *= vec4(isInBorder(textureCoords.x, u_Pixelborder.x), 0.0f, 0.0f, 1.0f);
    
}