#version 400 core
// Data passed in from the vertex shader
in vData
{
    vec2 textureCoords;
};

out vec4 o_Colour;

uniform sampler2D u_FragmentTexture;
uniform vec4 u_ColourTint;
uniform int u_DebugMode;

// Definition for the thickness of the border
// as shown on screen (in pixels)
uniform vec2 u_Pixelborder;

// Definition for the thickness of the border 
// as a proportion of texture (in pixels)
uniform float u_Textureborder;

// Texture x, y, width, height info in atlas-space
uniform vec4 u_TextureCoords;

// Converts coordinates from atlas space to 0-1 space, or vice-versa
vec2 convertCoordinates(vec2 coords, int direction) {
    if (direction == 0) {
        // Converting from real to 0-1
        return vec2(
            (coords.x - u_TextureCoords.x) / u_TextureCoords.z,
            (coords.y - u_TextureCoords.y) / u_TextureCoords.w
        );
    }else if (direction == 1) {
        // Converting from 0-1 to real
        return vec2(
            u_TextureCoords.x + (u_TextureCoords.z * coords.x),
            u_TextureCoords.y + (u_TextureCoords.w * coords.y)
        );
    }
}

float remapValue(float coord, float pixelBorder) {
    // Different logic for:
    //      start -> border edge
    //      border edge -> halfway
    //      halfway -> border edge
    //      border edge -> end
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
    // First convert coords from real (atlas-space) to 0-1
    // Then remap the coords
    // Then convert coords from 0-1 back to real
    vec2 normCoords = convertCoordinates(textureCoords, 0);
    vec2 newUV = vec2(remapValue(normCoords.x, u_Pixelborder.x), remapValue(normCoords.y, u_Pixelborder.y));
    vec2 realCoords = convertCoordinates(newUV, 1);
    
    // Debug mode colours based on remapped coords
    if (u_DebugMode == 1) {
        o_Colour = vec4(newUV.x, newUV.y, 0.0f, 1.0f);
        return;
    }
    
    // Print the texture after applying any tinting
    o_Colour = texture(u_FragmentTexture, realCoords);
    o_Colour *= u_ColourTint;
    
}