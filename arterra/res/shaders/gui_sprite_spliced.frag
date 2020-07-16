#version 400 core
in vData
{
    vec2 textureCoords;
};

out vec4 o_Colour;

uniform sampler2D u_FragmentTexture;
uniform vec4 u_ColourTint;
uniform vec2 u_Pixelborder;
uniform float u_Textureborder;
uniform int u_DebugMode;

float isInBorder(float coord, float pixelBorder) {
    if (coord < pixelBorder) {
        float percent = 1 - ((pixelBorder - coord) / pixelBorder);
        return u_Textureborder * percent;
    }else if (coord > (1-pixelBorder)) {
        float percent = 1 - ((pixelBorder - (1-coord)) / pixelBorder);
        return 1 - (u_Textureborder * percent);
    }else if (coord < 0.5) {
        float stretchLength = 0.5-pixelBorder;
        float percent = (coord-pixelBorder) * (1/stretchLength);
        return ((stretchLength * percent)*0.5) + u_Textureborder;
    }else if (coord > 0.5) {
        float stretchLength = 0.5-pixelBorder;
        float percent = (coord-0.5) * (1/stretchLength);
        return 0.5+((0.5-u_Textureborder) * percent);
    }
}

void main(void) {
    
    if (u_DebugMode == 0) {
        vec2 newUV = vec2(isInBorder(textureCoords.x, u_Pixelborder.x), isInBorder(textureCoords.y, u_Pixelborder.y));
        o_Colour = texture(u_FragmentTexture, newUV);
        o_Colour *= u_ColourTint;
    }else {
        vec2 newUV = vec2(isInBorder(textureCoords.x, u_Pixelborder.x), isInBorder(textureCoords.y, u_Pixelborder.y));
        o_Colour = vec4(newUV.x, newUV.y, 0.0f, 1.0f);
    }
    
}