#version 400 core
in vData
{
    vec2 textureCoords;
};

out vec4 o_colour;

uniform sampler2D fragmentTexture;
uniform vec4 colourTint;

// Dimensions of the input quad
uniform vec2 u_dimensions;
// Border width as a decimal of the texture width/height
// Can also be a vec2 for differently sized borders
uniform float u_border;

// Calculate the borders by dividing border by the width
vec2 borders = vec2(u_border) / u_dimensions.xx;

// Use borders.x for the x axis and borders.y for the y axis


float map(float value, float originalMin, float originalMax, float newMin, float newMax) {
    return (value - originalMin) / (originalMax - originalMin) * (newMax - newMin) + newMin;
} 


// Helper function, because WET code is bad code
// Takes in the coordinate on the current axis and the borders 
float processAxis(float coord, float textureBorder, float windowBorder) {
    // If the coordinate is in the (left/bottom) border region
    if (coord < windowBorder)
        // Return ( (coord / windowBorder) * (textureBorder) ) + textureBorder;
        // The coordinate as a ratio of the window, normalised to the texture border
        return map(coord, 0, windowBorder, 0, textureBorder) ;
        
    // If the coordinate is in the (right/top) border region
    if (coord < 1 - windowBorder) 
        // Same as before, but adjusted so that the area defined is the correct part of the texture
        // Ie.  The top or right border region
        return map(coord,  windowBorder, 1 - windowBorder, textureBorder, 1 - textureBorder);

    // If neither of the prior two, return the coordinate normalised to the inner area of the 9-slice
    return map(coord, 1 - windowBorder, 1, 1 - textureBorder, 1);
} 

void main(void) {
    // Calculate the borders by dividing border by the width
    borders = vec2(u_border) / u_dimensions.xx;
    // Calculate the new UV coordinates by using the function
    vec2 newUV = vec2(
        processAxis(textureCoords.x, u_border, borders.x),
        processAxis(textureCoords.y, u_border, borders.y)
    );
    // Output the color
    o_colour = texture(fragmentTexture, newUV);
}