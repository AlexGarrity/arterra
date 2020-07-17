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

// Rotation of element in radians between 0 and PI.
uniform float u_Rotation;

// Definition for the thickness of the border
// as shown on screen (in pixels relative to the equivalent dimension).
// E.g. sprite width = 500, pixelBorder = 50, pixelBorder.x = 50/500 = 0.1
uniform vec2 u_Pixelborder;

// Definition for the thickness of the border 
// as a proportion of the texture (in pixels).
// E.g. texture width = 16, pixel border with = 5, textureBorder.x = 5/16 = 0.3125
uniform float u_Textureborder;

// Texture x, y, width, height info in atlas-space.
// x: left-most point, y: bottom-most point
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
    // Convert pixelBorder values between x<->y to account for 0-90 rotation,
    // since the pixelBorder values are sprite-axis dependant.
    // E.g. 
    //      at rot: 0.0, x: x,          y: y
    //      at rot: 0.5, x: 1/2(y-x)+x, y: 1/2(x-y)+y
    //      at rot: 1.0, x: y,          y: x
    // Convert rotation of sprite: 0-PI to percent: 0-1-0, since
    // percentage scaling is on a 0-90 degrees basis.
    //
    // Non-equation logic for understanding:
    //      float scale;
    //      if (u_Rotation > 1.5708) {
    //          scale = 1 - ((u_Rotation - 1.5708) / 1.5708);
    //      }else {
    //          scale = u_Rotation / 1.5708;
    //      }
    float scale = 1-abs((u_Rotation-1.5708)/1.5708);
    vec2 newPixelBorder = vec2(
        ((u_Pixelborder.y-u_Pixelborder.x) * scale) + u_Pixelborder.x,
        ((u_Pixelborder.x-u_Pixelborder.y) * scale) + u_Pixelborder.y
    );
    
    // First convert coords from real (atlas-space) to 0-1.
    // Then remap the coords to get the 9-slice working.
    // Then convert coords from 0-1 back to real.
    vec2 normCoords = convertCoordinates(textureCoords, 0);
    vec2 newUV = vec2(remapValue(normCoords.x, newPixelBorder.x), remapValue(normCoords.y, newPixelBorder.y));
    vec2 realCoords = convertCoordinates(newUV, 1);
    
    // Debug mode colours based on remapped coords.
    if (u_DebugMode == 1) {
        o_Colour = vec4(newUV.x, newUV.y, 0.0f, 1.0f);
        return;
    }
    
    // Print the texture after applying any tinting.
    o_Colour = texture(u_FragmentTexture, realCoords);
    o_Colour *= u_ColourTint;
    
}